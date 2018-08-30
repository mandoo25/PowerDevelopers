// ***************************************************************************
// Copyright (C) 2005 Cognex Corporation
//
// Subject to Cognex Corporation's terms and conditions and license agreement,
// you are authorized to use and modify this source code in any way you find
// useful, provided the Software and/or the modified Software is used solely
// in conjunction with a Cognex Machine Vision System.  Furthermore you
// acknowledge and agree that Cognex has no warranty, obligations or liability
// for your use of the Software.
// ***************************************************************************

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;
using MySql.Data.MySqlClient;

using Cognex.VisionPro;
using Cognex.VisionPro.QuickBuild;
using Cognex.VisionPro.Display;
using Cognex.VisionPro.ToolGroup;
using Cognex.VisionPro.PMAlign;
using Cognex.VisionPro.ImageFile;
using Cognex.VisionPro.ID;



namespace EfficientApp
{
    enum CmdType
    {
        CMD_TYPE_REQUEST    = 0x20,
        CMD_TYPE_ACK        = 0x21,
        CMD_TYPE_NACK       = 0x22
    }

    enum ActionType
    {
        barcode1d    = 0x01,
        barcode2d    = 0x02,
        barcodeqr    = 0x03,
        exist        = 0x04,
        judgement    = 0x05
    }

    enum WorkItems
    {
        order = 0x00,
        item1 = 0x01,
        item2 = 0x02
    }

    enum LogType
    {
        info = 1,
        err = 2
    }

    enum PacketReturn
    {
        ok = 0,
        ng = 1,
        cont = 2
    }


      /// <summary>
      /// Summary description for Form1.
      /// </summary>
    public class Form1 : System.Windows.Forms.Form
    {
        public class Constants
        {
            public const int ReqCmdPckSize = 11;

            public const int order1stClassLength = 8;
            public const int order2ndClassLength = 2;
            public const int orderSerialNumLength = 8;
            public const int order1stClassPos = 2;
            public const int order2ndClassPos = 12;
            public const int orderSerialNumPos = 16;
        }

        public struct ReqCmd
        {
            public byte cmd_type;
            public byte action_type;
            public byte item_id;
            public byte cell_number;
            public byte process_number;
            public byte accracy;
            public byte order_size;
            public String order_str;
            public int image_size;
            public byte[] image;
            public String product_str;
            public String serial_str;
            public String cur_date;
            public String start_time;
            public String end_time;
            public long due_msec;
            public String saveFilePath;

            public int analysis_packet(byte[] buf, int size)
            {
                if (size < Constants.ReqCmdPckSize)
                {
                    return (int)PacketReturn.ng;
                }

                int i = 0;

                this.cmd_type = buf[i++];
                if (this.cmd_type != (byte)CmdType.CMD_TYPE_REQUEST)
                {
                    return (int)PacketReturn.ng;
                }

                this.action_type = buf[i++];
                this.item_id = buf[i++];
                this.cell_number = buf[i++];
                this.process_number = buf[i++];
                this.accracy = buf[i++];
                this.order_size = buf[i++];

                if (this.order_size > 0)
                {
                    if((this.order_size+i) > (size-4))
                    {
                        return (int)PacketReturn.ng;
                    }
                    
                    byte[] tmp_order_str = new byte[this.order_size];
                    System.Buffer.BlockCopy(buf, i, tmp_order_str, 0, this.order_size);
                    this.order_str = Encoding.ASCII.GetString(tmp_order_str, 0, this.order_size);
                    i = i + this.order_size;

                    OrderContents orderCont = new OrderContents();
                    if (orderCont.analysis(this.order_str) == 0)
                    {
                        this.product_str = orderCont.first_class + orderCont.second_class;
                        this.serial_str = orderCont.SerialNum;
                    }
                    else
                    {
                        return (int)PacketReturn.ng;
                    }
                }
                else if (this.item_id != (byte)WorkItems.order)
                {
                    return (int)PacketReturn.ng;
                }

                this.image_size = BitConverter.ToInt32(buf, i);
                i = i + 4;

                if (this.image_size > 0)
                {
                    if (this.image_size + i > size)
                    {
                        return (int)PacketReturn.cont;
                    }

                    this.image = new byte[this.image_size];
                    System.Buffer.BlockCopy(buf, i, this.image, 0, this.image_size);
                }

                return (int)PacketReturn.ok;
            }
        }

        public struct RespAck
        {
            public byte cmd_type;
            public byte action_type;
            public byte item_id;
            public byte cell_number;
            public byte process_number;
            public byte data_size;
            public String data;
        }

        public struct OrderContents
        {
            public string first_class;
            public string second_class;
            public string SerialNum;

            public int analysis(String str)
            {
                if(str.Length < 24)
                {
                    return -1;
                }

                this.first_class = str.Substring(Constants.order1stClassPos, Constants.order1stClassLength);
                this.second_class = str.Substring(Constants.order2ndClassPos, Constants.order2ndClassLength);
                this.SerialNum = str.Substring(Constants.orderSerialNumPos, Constants.orderSerialNumLength);

                return 0;
            }
        }
               

        String refDirectory = Application.StartupPath;
        String verificationDirectory = Application.StartupPath + @"\images\verification\";
        String backupDirectory = Application.StartupPath + @"\images\backup\";
        String verificationFile = "verification.jpg";
        String vppFilePath = @"D:\Source\VisionPro_Test\FrontCover.vpp";
        String logBackupPath = Application.StartupPath + @"\LOG";
        private StreamWriter _write;
        private FileStream _fs;
        MySqlConnection mySqlConn;
        internal System.Windows.Forms.Label Label1;
        internal System.Windows.Forms.TextBox myCountText;
        internal System.Windows.Forms.TextBox SampleTextBox;
        private delegate void UpdateString(string text);
        CogJobManager myJobManager;
        CogJob myJob;
        CogJobIndependent myIndependentJob;
        private CogRecordDisplay cogRecordDisplay1;
        private TextBox HostNameTextBox;
        private Label HostNameLabel;
        private IContainer components;
        private object _lock = new object();
        private Label label3;
        private NumericUpDown PortNumberBox;
        private Button ListenButton; //QuickBuild중복 실행 방지용
        private TcpListener _listener;
        private TextBox OutputTextBox;
        private Thread _connectionThread;

        private List<Thread> _threads = new List<Thread>();
        private List<TcpClient> _clients = new List<TcpClient>();
        private List<NetworkStream> _streams = new List<NetworkStream>();
        private object _listLock = new object();
        private object _CogLock = new object();
        private int stopping;
        private int save_wait_f;
        private CheckBox checkBox1;
        private GroupBox groupBox1;
        private Label label2;
        private TextBox textBox1;
        private Button DirSelect;
        private Stopwatch stopWatch = new Stopwatch();

        //		C# is a multi-threaded language, unlike VB6. Because of this, one must be careful
        //		when worker threads interact with the GUI (which is on its own thread). One preferred
        //		way to do this in .NET is to use the InvokeRequired/BeginInvoke() mechanisms.  These 
        //		mechanisms allow worker threads to tell the GUI thread that a given method needs to be
        //		run on the GUI thread.  This insures thread safety on the GUI thread.  Otherwise, bad
        //		things (crashes, etc.) might occur if non-GUI threads tried to update the GUI.
        //
        //		When InvokeRequired is called from a Form's method, it determines if the calling thread
        //		is different from the Form's thread. If so, it returns true which indicates that 
        //		a worker thread wants to post something to the GUI.  Else, it returns false.
        //
        //		If InvokeRequired is true, then the caller is trying to tell the GUI thread to run a  
        //		particular method to post something on the GUI.  To do this, one employs BeginInvoke with
        //		a delegate that contains the address of the particular method to run along with parameters needed
        //		by that method.
        //

        //		Delegates which dictate the signature of the methods that will post to the GUI.
        delegate void UserResultDelegate(object sender, CogJobManagerActionEventArgs e);

        public Form1()
        {
            //
            // Required for Windows Form Designer support
            //
            InitializeComponent();
            InitializeJobManager();
            
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            foreach(IPAddress ip in host.AddressList)
            {
                if(ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    HostNameTextBox.Text = ip.ToString();
                }
            }      

            this.Closing += new CancelEventHandler(Form1_Closing);
        }

        private void InitializeJobManager()
        {
            /*
            SampleTextBox.Text =
            "This sample demonstrates how to load a persisted QuickBuild application and access " +
            "the results provided in the posted items (a.k.a. as user result queue)." +
            System.Environment.NewLine + System.Environment.NewLine +

            "The sample uses mySavedQB.vpp, which consists of a single Job that executes a " +
            "Blob tool with default parameters using a frame grabber provided image.  " +
            @"The provided .vpp file is configured to use ""VPRO_ROOT\images\pmSample.idb"" as the " +
            "source of images." +
            System.Environment.NewLine + System.Environment.NewLine +
            "To use:  Click the Run button or the Run Continuous button.  " +
            "The number of blobs will be displayed in the count text box " +
            "and the Blob tool input image will be displayed in the image display control.";             
             * */

            //Depersist the QuickBuild session
        //JS TEST
            /*
            myJobManager = (CogJobManager)CogSerializer.LoadObjectFromFile(
                Environment.GetEnvironmentVariable("VPRO_ROOT") + 
                "\\Samples\\Programming\\QuickBuild\\mySavedQB.vpp");
                * */
            /*
            myJobManager = (CogJobManager)CogSerializer.LoadObjectFromFile("D:\\Source\\VisionPro_Test\\FrontCover.vpp");
            //myJob = myJobManager.Job("FrontCover_1_10cm");
            //myIndependentJob = myJob.OwnedIndependent;

            //flush queues
            myJobManager.UserQueueFlush();
            myJobManager.FailureQueueFlush();
            //myJob.ImageQueueFlush();
            //myIndependentJob.RealTimeQueueFlush();

            // setup event handlers.  These are called when a result packet is available on
            // the User Result Queue or the Real-Time Queue, respectively.
            myJobManager.UserResultAvailable += new CogJobManager.CogUserResultAvailableEventHandler(myJobManager_UserResultAvailable);
                * */

        }

        private void InitializeDataBase()
        {
            string MyConString = "Server=127.0.0.1; Port=3308; Database=log; Uid=root; Password=root; SslMode=none";
            mySqlConn = new MySqlConnection(MyConString);
            mySqlConn.Open();

            /*
            MySqlCommand command = mySqlConn.CreateCommand();
            MySqlDataReader Reader;

            command.CommandText = "select * from k595np";
            
            Reader = command.ExecuteReader();

            StringBuilder sb = new StringBuilder();

            while(Reader.Read())
            {
                string thisrow = "";
                for(int i=0; i< Reader.FieldCount; i++)
                {
                    thisrow += Reader.GetValue(i).ToString() + ",";
                }
                sb.AppendLine(thisrow);
            }
            //mySqlConn.Close();

            SampleTextBox.Text = sb.ToString();
             * */
        }

        //	If it is called by a worker thread,
        //	InvokeRequired is true, as described above.  When this occurs, a delegate is constructed
        //	which is really a pointer to the method that the GUI thread should call.
        //	BeginInvoke is then called, with this delegate and the Image parameter.
        //	Notice that this subroutine tells the GUI thread to call the same subroutine!  
        //	When the GUI calls this method on its own thread, InvokeRequired will be false and the 
        //	CogRecordDisplay is updated with the info.
        // This method handles the UserResultAvailable Event. The user packet
        // has been configured to contain the blob tool input image, which we retrieve and display.
        private void myJobManager_UserResultAvailable(object sender, CogJobManagerActionEventArgs e)
        {
            if (this.InvokeRequired)
            {
            this.BeginInvoke(new UserResultDelegate(myJobManager_UserResultAvailable), new object[] { sender, e });
            return;
            }
            Cognex.VisionPro.ICogRecord tmpRecord;
            Cognex.VisionPro.ICogRecord topRecord = myJobManager.UserResult();

            // check to be sure results are available
            if (topRecord == null) return;

            /* JS TEST
            // Assume that the required "count" record is present, and go get it.
            tmpRecord = topRecord.SubRecords[@"Tools.Item[""CogBlobTool1""].CogBlobTool.Results.GetBlobs().Count"];
            int count = (int)tmpRecord.Content;
            myCountText.Text = count.ToString();

            // Assume that the required "image" record is present, and go get it.
            tmpRecord = topRecord.SubRecords["ShowLastRunRecordForUserQueue"];
            tmpRecord = tmpRecord.SubRecords["LastRun"];
            tmpRecord = tmpRecord.SubRecords["Image Source.OutputImage"];
            cogRecordDisplay1.Record = tmpRecord;
            cogRecordDisplay1.Fit(true);
            * */
            tmpRecord = topRecord.SubRecords["ShowLastRunRecordForUserQueue"];
            tmpRecord = tmpRecord.SubRecords["LastRun"];
            tmpRecord = tmpRecord.SubRecords["CogImageConvertTool1.OutputImage"];
            cogRecordDisplay1.Record = tmpRecord;      
            cogRecordDisplay1.Fit(true);

            //String strFileTmp = string.Format("D:\\test_image\\JS_TEST_{0}.jpg", 6);
            /*
            DirectoryInfo di = new DirectoryInfo(saveFilePath);
            if (di.Exists == false)
            {
                di.Create();
            }

            String strFileTmp = saveFilePath + saveFileName;
            cogRecordDisplay1.CreateContentBitmap(CogDisplayContentBitmapConstants.Image).Save(strFileTmp, System.Drawing.Imaging.ImageFormat.Jpeg);
            UpdateGUI("save" + saveFileName);
             * */

            save_wait_f = 0;
        }


        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
            if (components != null)
            {
                components.Dispose();
            }
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.Label1 = new System.Windows.Forms.Label();
            this.myCountText = new System.Windows.Forms.TextBox();
            this.SampleTextBox = new System.Windows.Forms.TextBox();
            this.cogRecordDisplay1 = new Cognex.VisionPro.CogRecordDisplay();
            this.HostNameTextBox = new System.Windows.Forms.TextBox();
            this.HostNameLabel = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.PortNumberBox = new System.Windows.Forms.NumericUpDown();
            this.ListenButton = new System.Windows.Forms.Button();
            this.OutputTextBox = new System.Windows.Forms.TextBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.DirSelect = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.cogRecordDisplay1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PortNumberBox)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // Label1
            // 
            this.Label1.Location = new System.Drawing.Point(10, 400);
            this.Label1.Name = "Label1";
            this.Label1.Size = new System.Drawing.Size(48, 19);
            this.Label1.TabIndex = 4;
            this.Label1.Text = "Status";
            // 
            // myCountText
            // 
            this.myCountText.Location = new System.Drawing.Point(41, 422);
            this.myCountText.Name = "myCountText";
            this.myCountText.ReadOnly = true;
            this.myCountText.Size = new System.Drawing.Size(86, 21);
            this.myCountText.TabIndex = 3;
            // 
            // SampleTextBox
            // 
            this.SampleTextBox.Location = new System.Drawing.Point(139, 449);
            this.SampleTextBox.Multiline = true;
            this.SampleTextBox.Name = "SampleTextBox";
            this.SampleTextBox.ReadOnly = true;
            this.SampleTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.SampleTextBox.Size = new System.Drawing.Size(526, 172);
            this.SampleTextBox.TabIndex = 5;
            // 
            // cogRecordDisplay1
            // 
            this.cogRecordDisplay1.ColorMapLowerClipColor = System.Drawing.Color.Black;
            this.cogRecordDisplay1.ColorMapLowerRoiLimit = 0D;
            this.cogRecordDisplay1.ColorMapPredefined = Cognex.VisionPro.Display.CogDisplayColorMapPredefinedConstants.None;
            this.cogRecordDisplay1.ColorMapUpperClipColor = System.Drawing.Color.Black;
            this.cogRecordDisplay1.ColorMapUpperRoiLimit = 1D;
            this.cogRecordDisplay1.DoubleTapZoomCycleLength = 2;
            this.cogRecordDisplay1.DoubleTapZoomSensitivity = 2.5D;
            this.cogRecordDisplay1.Location = new System.Drawing.Point(139, 120);
            this.cogRecordDisplay1.MouseWheelMode = Cognex.VisionPro.Display.CogDisplayMouseWheelModeConstants.Zoom1;
            this.cogRecordDisplay1.MouseWheelSensitivity = 1D;
            this.cogRecordDisplay1.Name = "cogRecordDisplay1";
            this.cogRecordDisplay1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("cogRecordDisplay1.OcxState")));
            this.cogRecordDisplay1.Size = new System.Drawing.Size(526, 323);
            this.cogRecordDisplay1.TabIndex = 6;
            // 
            // HostNameTextBox
            // 
            this.HostNameTextBox.BackColor = System.Drawing.SystemColors.Control;
            this.HostNameTextBox.Location = new System.Drawing.Point(83, 12);
            this.HostNameTextBox.Name = "HostNameTextBox";
            this.HostNameTextBox.Size = new System.Drawing.Size(203, 21);
            this.HostNameTextBox.TabIndex = 7;
            // 
            // HostNameLabel
            // 
            this.HostNameLabel.AutoSize = true;
            this.HostNameLabel.Location = new System.Drawing.Point(10, 15);
            this.HostNameLabel.Name = "HostNameLabel";
            this.HostNameLabel.Size = new System.Drawing.Size(71, 12);
            this.HostNameLabel.TabIndex = 8;
            this.HostNameLabel.Text = "IP Address:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(310, 15);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(62, 12);
            this.label3.TabIndex = 8;
            this.label3.Text = "Port Num:";
            // 
            // PortNumberBox
            // 
            this.PortNumberBox.Location = new System.Drawing.Point(378, 12);
            this.PortNumberBox.Maximum = new decimal(new int[] {
            65536,
            0,
            0,
            0});
            this.PortNumberBox.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PortNumberBox.Name = "PortNumberBox";
            this.PortNumberBox.Size = new System.Drawing.Size(120, 21);
            this.PortNumberBox.TabIndex = 9;
            this.PortNumberBox.Value = new decimal(new int[] {
            5001,
            0,
            0,
            0});
            // 
            // ListenButton
            // 
            this.ListenButton.Location = new System.Drawing.Point(513, 5);
            this.ListenButton.Name = "ListenButton";
            this.ListenButton.Size = new System.Drawing.Size(85, 32);
            this.ListenButton.TabIndex = 10;
            this.ListenButton.Text = "Listen";
            this.ListenButton.UseVisualStyleBackColor = true;
            this.ListenButton.Click += new System.EventHandler(this.ListenButton_Click);
            // 
            // OutputTextBox
            // 
            this.OutputTextBox.AcceptsReturn = true;
            this.OutputTextBox.BackColor = System.Drawing.SystemColors.Control;
            this.OutputTextBox.ImeMode = System.Windows.Forms.ImeMode.Hangul;
            this.OutputTextBox.Location = new System.Drawing.Point(671, 82);
            this.OutputTextBox.Multiline = true;
            this.OutputTextBox.Name = "OutputTextBox";
            this.OutputTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.OutputTextBox.Size = new System.Drawing.Size(275, 323);
            this.OutputTextBox.TabIndex = 11;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = true;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.Location = new System.Drawing.Point(12, 50);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(69, 16);
            this.checkBox1.TabIndex = 12;
            this.checkBox1.Text = "Logging";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.DirSelect);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBox1);
            this.groupBox1.Location = new System.Drawing.Point(12, 50);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(474, 47);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "groupBox1";
            // 
            // DirSelect
            // 
            this.DirSelect.Location = new System.Drawing.Point(378, 16);
            this.DirSelect.Name = "DirSelect";
            this.DirSelect.Size = new System.Drawing.Size(75, 23);
            this.DirSelect.TabIndex = 2;
            this.DirSelect.Text = "...";
            this.DirSelect.UseVisualStyleBackColor = true;
            this.DirSelect.Click += new System.EventHandler(this.DirSelect_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(55, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "Directory";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(72, 18);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(288, 21);
            this.textBox1.TabIndex = 0;
            this.textBox1.Text = logBackupPath;
            // 
            // Form1
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
            this.ClientSize = new System.Drawing.Size(959, 633);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.OutputTextBox);
            this.Controls.Add(this.ListenButton);
            this.Controls.Add(this.PortNumberBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.HostNameLabel);
            this.Controls.Add(this.HostNameTextBox);
            this.Controls.Add(this.cogRecordDisplay1);
            this.Controls.Add(this.SampleTextBox);
            this.Controls.Add(this.myCountText);
            this.Controls.Add(this.Label1);
            this.Name = "Form1";
            this.Text = "QuickBuild Sample Application";
            ((System.ComponentModel.ISupportInitialize)(this.cogRecordDisplay1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PortNumberBox)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }
        #endregion

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.Run(new Form1());
        }

        private void Form1_Closing(object sender, CancelEventArgs e)
        {
            if (ListenButton.Text != "Listen")
            {
                myJobManager.UserResultAvailable -= new CogJobManager.CogUserResultAvailableEventHandler(myJobManager_UserResultAvailable);
                cogRecordDisplay1.Dispose();
                // Be sure to shudown the CogJobManager!!
                myJobManager.Shutdown();
                mySqlConn.Close();
            }            
        }      

        
        private void ConnectToServer()
        {
            try
            {
                ListenButton.Text = "Stop";

                InitializeDataBase();
                // There is only one connection thread that is used to connect clients.
                _connectionThread = new System.Threading.Thread(new ThreadStart(ConnectToClient));
                _connectionThread.IsBackground = true;
                _connectionThread.Start();
                PortNumberBox.Enabled = false;
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, ex.Message);
                print_log((byte)LogType.err, ex.Message);
            }            
        }

        private void StopServer()
        {
            Cursor.Current = Cursors.WaitCursor;

            ListenButton.Text = "Listen";

            if (_listener != null)
            {
                _listener.Stop();
                _connectionThread.Join();

                lock (_listLock)
                {
                    stopping = 1;
                    // close all client streams
                    foreach (NetworkStream s in _streams)
                    {
                        //MessageBox.Show("stream close");
                        s.Close();
                    }
                    _streams.Clear();

                    // close the client connection
                    foreach (TcpClient client in _clients)
                    {
                        //MessageBox.Show("client close");
                        client.Close();
                    }
                    _clients.Clear();
                }

                foreach(Thread t in _threads)
                {
                    t.Join();
                }
                _threads.Clear();
            }

            PortNumberBox.Enabled = true;
            Cursor.Current = Cursors.Default;

            myJobManager.UserResultAvailable -= new CogJobManager.CogUserResultAvailableEventHandler(myJobManager_UserResultAvailable);        
            // Be sure to shudown the CogJobManager!!
            myJobManager.Shutdown();
            mySqlConn.Close();
        }

        private void ConnectToClient()
        {
            try
            {
                // Create TCPListener to start listening
                _listener = new TcpListener(IPAddress.Any,
                    Int32.Parse(PortNumberBox.Value.ToString()));

                // Initiates the underlying socket, binds it to the local endpoint, 
                // and begins listening for incoming connection attempts.
                _listener.Start();
            }
            catch (SocketException se)
            {
                DisplayError(se.ErrorCode.ToString() + ": " + se.Message);
                StopServer();
                return;
            }

            try
            {
                for (; ; )
                {
                    // Wait for a client connection request
                    TcpClient client = _listener.AcceptTcpClient();
                    // Create a thread to start accepting client's data.
                    Thread t = new Thread(new ParameterizedThreadStart(ReceiveDataFromClient));
                    t.IsBackground = true;
                    t.Priority = ThreadPriority.AboveNormal;

                    _threads.Add(t);
                    _clients.Add(client);

                    t.Start(client);               

                }
            }
            catch (SocketException ex)
            {
                // Display an error message unless we intentionally close server.
                if (ex.ErrorCode != (int)SocketError.Interrupted)
                    DisplayError("Lost connection due to the following error: " + ex.Message);
            }
            catch (Exception ex)
            {
                DisplayError("Lost connection due to the following error: " + ex.Message);
            }
        }

        public void backupImages(ref ReqCmd reqCmd, string serial_num, bool result)
        {
            string saveFilePath = backupDirectory + reqCmd.cur_date + "\\";
            string saveFileName = string.Format("item{0}.jpg", reqCmd.item_id);
            //DateTime now = DateTime.Now;

            if (serial_num != null) saveFilePath = saveFilePath + serial_num + "\\";

            DirectoryInfo di = new DirectoryInfo(saveFilePath);
            if (di.Exists == false)
            {
                di.Create();
            }

            String strFileTmp;
            if (result == true && serial_num != null)
            {                
                strFileTmp = saveFilePath + saveFileName; 
                FileInfo fi = new FileInfo(strFileTmp);
                if (fi.Exists)
                {
                    di = new DirectoryInfo(saveFilePath + "old");
                    if (di.Exists == false)
                    {
                        di.Create();
                    }

                    //fi.CopyTo(saveFilePath + "old\\" + saveFileName, true);
                    fi.MoveTo(saveFilePath + "old\\" + reqCmd.cur_date + "_" + reqCmd.start_time + "_" + saveFileName);
                    //fi.Delete();
                }
            }
            else
            {
                di = new DirectoryInfo(saveFilePath + "NG");
                if (di.Exists == false)
                {
                    di.Create();
                }
                strFileTmp = saveFilePath + "NG\\" + reqCmd.cur_date + "_" + reqCmd.start_time + "_" 
                    + string.Format("cell{0}_process{1}_", reqCmd.cell_number, reqCmd.process_number) + saveFileName;
            }

            cogRecordDisplay1.CreateContentBitmap(CogDisplayContentBitmapConstants.Image).Save(strFileTmp, System.Drawing.Imaging.ImageFormat.Jpeg);
            reqCmd.saveFilePath = strFileTmp;
            print_log((byte)LogType.info, "File Saved at " + strFileTmp);
            UpdateGUI("save " + saveFileName);  
        }

        public RespAck CogActBarcode(ref ReqCmd reqCmd)
        {
            RespAck respAck = new RespAck();
            
            //MessageBox.Show(string.Format("{0}", respAck.cmd_type));
            try
            {
                //myJob = myJobManager.Job("Barcode_1D");
                CogToolGroup myTG = myJob.VisionTool as CogToolGroup;
                CogImageFileTool myIFTool = myTG.Tools["CogImageFileTool1"] as CogImageFileTool;

                //string openFilePath = refDirectory + string.Format("verification\\cell{0}\\main{1}\\", reqCmd.cell_number, reqCmd.process_number);
                string openFilePath = verificationDirectory;
                //string openFileName = Enum.GetName(typeof(ActionType), reqCmd.action_type) + "_" + Enum.GetName(typeof(WorkItems), reqCmd.item_id) + ".jpg";
                string openFileName = verificationFile;

                //MessageBox.Show(openFilePath + openFileName);
                myIFTool.Operator.Open(openFilePath + openFileName, CogImageFileModeConstants.Read);



                myJob.Run();
                UpdateGUI("Run");

                save_wait_f = 1;
                while (save_wait_f == 1)
                {
                    Thread.Sleep(1);
                }


                CogIDTool myIDTool = myTG.Tools["CogIDTool1"] as CogIDTool;
                if (myIDTool.Results.Count > 0)
                {
                    string resultId = myIDTool.Results[0].DecodedData.DecodedString;
                    //결과값에 Group Separator문자(0x29) 삭제
                    resultId = resultId.Replace("\u001d", "");
                    //MessageBox.Show(resultId);
                    UpdateGUI(resultId);
                    print_log((byte)LogType.info, resultId);
                    print_log((byte)LogType.info, string.Format("length : {0}", resultId.Length));
                    respAck.cmd_type = (byte)CmdType.CMD_TYPE_ACK;
                    respAck.action_type = reqCmd.action_type;
                    respAck.item_id = reqCmd.item_id;
                    respAck.cell_number = reqCmd.cell_number;
                    respAck.process_number = reqCmd.process_number;

                    respAck.data_size = (byte)resultId.Length;
                    respAck.data = resultId;


                    if (reqCmd.item_id == (byte)WorkItems.order)
                    {
                        OrderContents orderCont = new OrderContents();
                        orderCont.analysis(resultId);
                        if(orderCont.SerialNum == null)
                        {
                            respAck.cmd_type = (byte)CmdType.CMD_TYPE_NACK;
                            print_log((byte)LogType.info, string.Format("orderCont.SerialNum NULL"));
                        }
                        backupImages(ref reqCmd, orderCont.SerialNum, true);
                        //if(orderCont.SerialNum != null) saveFilePath = saveFilePath + orderCont.SerialNum + "\\";
                    }
                    else
                    {
                        if (reqCmd.serial_str == null)
                        {
                            respAck.cmd_type = (byte)CmdType.CMD_TYPE_NACK;
                            print_log((byte)LogType.info, string.Format("reqCmd.serial_str"));
                        }
                        backupImages(ref reqCmd, reqCmd.serial_str, true);
                        //if (reqCmd.serial_str != null) saveFilePath = saveFilePath + reqCmd.serial_str + "\\";
                    }                   
                }
                else
                {
                    respAck.cmd_type = (byte)CmdType.CMD_TYPE_NACK;
                    print_log((byte)LogType.info, string.Format("myIDTool.Results.Count : {0}", myIDTool.Results.Count));
                    backupImages(ref reqCmd, null, false);
                }

                while(myJob.State != CogJobStateConstants.Stopped)
                {
                    Thread.Sleep(1);
                }


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                respAck.cmd_type = (byte)CmdType.CMD_TYPE_NACK;
                print_log((byte)LogType.err, ex.Message);
            }



            return respAck;
        }

        public RespAck CogActBarcode2D(ReqCmd reqCmd)
        {
            RespAck respAck = new RespAck();

            stopWatch.Start();

            //MessageBox.Show(string.Format("{0}", respAck.cmd_type));
            try
            {
                //myJob = myJobManager.Job("Barcode_2D");
                CogToolGroup myTG = myJob.VisionTool as CogToolGroup;
                CogImageFileTool myIFTool = myTG.Tools["CogImageFileTool1"] as CogImageFileTool;

                //string openFilePath = refDirectory + string.Format("verification\\cell{0}\\main{1}\\", reqCmd.cell_number, reqCmd.process_number);
                //string openFileName = Enum.GetName(typeof(ActionType), reqCmd.action_type) + "_" + Enum.GetName(typeof(WorkItems), reqCmd.item_id) + ".jpg";
                string openFilePath = verificationDirectory;
                string openFileName = verificationFile;

                //MessageBox.Show(openFilePath + openFileName);
                myIFTool.Operator.Open(openFilePath + openFileName, CogImageFileModeConstants.Read);



                myJob.Run();
                UpdateGUI("Run");

                save_wait_f = 1;
                while (save_wait_f == 1)
                {
                    Thread.Sleep(1);
                }


                CogIDTool myIDTool = myTG.Tools["CogIDTool1"] as CogIDTool;
                if (myIDTool.Results.Count > 0)
                {
                    string resultId = myIDTool.Results[0].DecodedData.DecodedString;
                    //결과값에 Group Separator문자(0x29) 삭제
                    resultId = resultId.Replace("\u001d", "");
                    //MessageBox.Show(resultId);
                    UpdateGUI(resultId);
                    print_log((byte)LogType.info, resultId);
                    print_log((byte)LogType.info, string.Format("length : {0}", resultId.Length));
                    respAck.cmd_type = (byte)CmdType.CMD_TYPE_ACK;
                    respAck.action_type = reqCmd.action_type;
                    respAck.item_id = reqCmd.item_id;
                    respAck.cell_number = reqCmd.cell_number;
                    respAck.process_number = reqCmd.process_number;

                    respAck.data_size = (byte)resultId.Length;
                    respAck.data = resultId;


                    if (reqCmd.item_id == (byte)WorkItems.order)
                    {
                        OrderContents orderCont = new OrderContents();
                        orderCont.analysis(resultId);
                        backupImages(ref reqCmd, orderCont.SerialNum, true);
                        //if(orderCont.SerialNum != null) saveFilePath = saveFilePath + orderCont.SerialNum + "\\";
                    }
                    else
                    {
                        backupImages(ref reqCmd, reqCmd.serial_str, true);
                        //if (reqCmd.serial_str != null) saveFilePath = saveFilePath + reqCmd.serial_str + "\\";
                    }
                }
                else
                {
                    respAck.cmd_type = (byte)CmdType.CMD_TYPE_NACK;
                    print_log((byte)LogType.info, string.Format("myIDTool.Results.Count : {0}", myIDTool.Results.Count));
                    backupImages(ref reqCmd, null, false);
                }

                while (myJob.State != CogJobStateConstants.Stopped)
                {
                    Thread.Sleep(1);
                }


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                respAck.cmd_type = (byte)CmdType.CMD_TYPE_NACK;
                print_log((byte)LogType.err, ex.Message);
            }

            stopWatch.Stop();
            UpdateGUI("Sleep ends" + stopWatch.ElapsedMilliseconds);
            stopWatch.Reset();

            return respAck;
        }

        public void CogActBarcodeQR()
        {

        }

        public void ReceiveDataFromClient(object clientObject)
        {
            TcpClient client = clientObject as TcpClient;
            NetworkStream netStream = null;
            int rtn = 0;

            try
            {
                netStream = client.GetStream();
            }
            catch (Exception ex)
            {
                // a bad connection, couldn't get the NetworkStream
                if (netStream != null) netStream.Close();
                DisplayError(ex.Message);
                return;
            }

            if(netStream.CanRead)
            {
                _streams.Add(netStream);
                try
                {
                    byte[] receiveBuffer = new byte[1024*500];                    
                    int bytesReceived;
                    int totalReceivedBytes = 0;

                    while ((bytesReceived = netStream.Read(receiveBuffer, totalReceivedBytes, (receiveBuffer.Length - totalReceivedBytes))) > 0)
                    {
                        //UpdateGUI(Encoding.ASCII.GetString(receiveBuffer, 0, bytesReceived));
                        totalReceivedBytes = totalReceivedBytes + bytesReceived;
                    
                        ReqCmd reqCmd = new ReqCmd();
                        RespAck respAck = new RespAck();

                        rtn = reqCmd.analysis_packet(receiveBuffer, totalReceivedBytes);
                        if(rtn == (int)PacketReturn.cont)
                        {
                            continue;
                        }
                        totalReceivedBytes = 0;

                        if (rtn == (int)PacketReturn.ok)
                        {
                            reqCmd.cur_date = DateTime.Today.ToString("yyyy-MM-dd");
                            reqCmd.start_time = DateTime.Now.ToString("HH_mm_ss_fff");
                            stopWatch.Start();

                            print_log((byte)LogType.info, string.Format("[Received Data] {0:X} {1:X} {2:X} {3:X} {4:X} {5:X} {6:X} ",
                                reqCmd.cmd_type, reqCmd.action_type, reqCmd.item_id, reqCmd.cell_number, reqCmd.process_number, reqCmd.accracy, reqCmd.order_size) + reqCmd.order_str + string.Format(" {0}", reqCmd.image_size));
                            //MessageBox.Show(reqCmd.data);
                            print_log((byte)LogType.info, "Product : " + reqCmd.product_str);
                            print_log((byte)LogType.info, "Serial Num : " + reqCmd.serial_str);


                            lock (_CogLock)
                            {
                                if (reqCmd.image_size != 0 && reqCmd.image != null)
                                {
                                    DirectoryInfo di = new DirectoryInfo(verificationDirectory);

                                    if (di.Exists == false)
                                    {
                                        di.Create();
                                    }

                                    File.WriteAllBytes(verificationDirectory+verificationFile, reqCmd.image);
                                }

                                switch (reqCmd.cmd_type)
                                {
                                    case (byte)CmdType.CMD_TYPE_REQUEST:
                                        {
                                            //MessageBox.Show("cmd_type_request");
                                            //DoCogSequence(rtn);
                                            switch (reqCmd.action_type)
                                            {
                                                case (byte)ActionType.barcode1d:
                                                    myJob = myJobManager.Job("Barcode_1D");
                                                    respAck = CogActBarcode(ref reqCmd);
                                                    break;

                                                case (byte)ActionType.barcode2d:
                                                    myJob = myJobManager.Job("Barcode_2D");
                                                    respAck = CogActBarcode(ref reqCmd);
                                                    break;

                                                default:
                                                    break;
                                            }
                                            break;
                                        }
                                    default:

                                        break;
                                }
                            }
                       

                            reqCmd.end_time = DateTime.Now.ToString("HH_mm_ss_fff");
                            stopWatch.Stop();
                            reqCmd.due_msec = stopWatch.ElapsedMilliseconds;
                            UpdateGUI("Sleep ends" + stopWatch.ElapsedMilliseconds);
                            stopWatch.Reset();

                        }
                        else
                        {
                            respAck.cmd_type = (byte)CmdType.CMD_TYPE_NACK;
                            respAck.action_type = reqCmd.action_type;
                            respAck.item_id = reqCmd.item_id;
                            respAck.cell_number = reqCmd.cell_number;
                            respAck.process_number = reqCmd.process_number;
                        }

                        if (netStream.CanWrite && respAck.cmd_type != 0)
                        {
                            byte[] responseBuffer = Encoding.ASCII.GetBytes("******" + respAck.data);

                            responseBuffer.SetValue(respAck.cmd_type, 0);
                            responseBuffer.SetValue(respAck.action_type, 1);
                            responseBuffer.SetValue(respAck.item_id, 2);
                            responseBuffer.SetValue(respAck.cell_number, 3);
                            responseBuffer.SetValue(respAck.process_number, 4);
                            responseBuffer.SetValue(respAck.data_size, 5);                            
                            
                            netStream.Write(responseBuffer, 0, responseBuffer.Length);                            
                            netStream.Flush();

                            if(respAck.cmd_type == (byte)CmdType.CMD_TYPE_ACK && reqCmd.product_str != null && reqCmd.serial_str != null)
                            {
                                string result_detail;
                                if(respAck.data_size == 0)
                                {
                                    result_detail = "OK";
                                }
                                else
                                {
                                    result_detail = respAck.data;
                                }

                                //MySqlCommand mySqlCommand = mySqlConn.CreateCommand();
                                //mySqlCommand.CommandText = "insert into k595np values('"+reqCmd.product_str+"', '"+reqCmd.serial_str+"', '"+respAck.action_type+"', '"+respAck.item_id+"', '"+result_detail+"', '"+ reqCmd.cur_date + "', '"+reqCmd.start_time+"', '"+reqCmd.end_time+"', '"+reqCmd.due_msec+"', '"+reqCmd.cell_number+"', '"+reqCmd.process_number+"', '"+reqCmd.saveFilePath+"');";
                                string sql = "INSERT into k595np VALUES(@ProductCode, @SerialNum, @ActionType, @WorkItems, @ResultDetail, @CheckDate, @StartTime, @EndTime, @TimeDiff, @CheckCell, @CheckProcess, @BackupImagePath);";
                                MySqlCommand cmd = new MySqlCommand(sql, mySqlConn);
                                cmd.Parameters.AddWithValue("@ProductCode", reqCmd.product_str);
                                cmd.Parameters.AddWithValue("@SerialNum", reqCmd.serial_str);
                                cmd.Parameters.AddWithValue("@ActionType", respAck.action_type);
                                cmd.Parameters.AddWithValue("@WorkItems", respAck.item_id);
                                cmd.Parameters.AddWithValue("@ResultDetail", result_detail);
                                cmd.Parameters.AddWithValue("@CheckDate", reqCmd.cur_date);
                                cmd.Parameters.AddWithValue("@StartTime", reqCmd.start_time);
                                cmd.Parameters.AddWithValue("@EndTime", reqCmd.end_time);
                                cmd.Parameters.AddWithValue("@TimeDiff", reqCmd.due_msec);
                                cmd.Parameters.AddWithValue("@CheckCell", reqCmd.cell_number);
                                cmd.Parameters.AddWithValue("@CheckProcess", reqCmd.process_number);
                                cmd.Parameters.AddWithValue("@BackupImagePath", reqCmd.saveFilePath);

                                cmd.ExecuteNonQuery();
                            }
                        }
                    }
                }
                catch(Exception ex)
                {
                    if(ListenButton.Text != "Listen")
                    {
                        DisplayError(ex.Message);
                    }
                }
            }

            lock (_listLock)
            {
                if (_streams.Contains(netStream))
                {
                    netStream.Close();
                    //MessageBox.Show("netstream remove");
                    _streams.Remove(netStream);
                }

                if (_clients.Contains(client))
                {
                    client.Close();
                    //MessageBox.Show("client remove");
                    _clients.Remove(client);
                }

                if (stopping == 0)
                {
                    if (_threads.Contains(Thread.CurrentThread))
                    {
                        _threads.Remove(Thread.CurrentThread);
                    }
                }
            }

        }

        private void UpdateGUI(string s)
        {
            if (OutputTextBox.InvokeRequired)
                OutputTextBox.BeginInvoke(new UpdateString(UpdateGUI), new object[] { s });
            else
            {
                if (OutputTextBox.TextLength >= OutputTextBox.MaxLength)
                    OutputTextBox.Text = "";
                OutputTextBox.AppendText(s);
                OutputTextBox.AppendText("\n");
            }
        }

        private void DisplayError(string message)
        {
            if (this.InvokeRequired)
                this.BeginInvoke(new UpdateString(DisplayError), new object[] { message });
            else
            {
                MessageBox.Show(this, message, "QuickBuild Server Sample");
                print_log((byte)LogType.err, message);
            }
        }

        private void OpenFile()
        {
            if(checkBox1.Checked)
            {
                CloseFile();

                DirectoryInfo di = new DirectoryInfo(textBox1.Text);

                if(di.Exists == false)
                {
                    di.Create();
                }

                string strFile = textBox1.Text + "\\" + DateTime.Today.ToString("yyyy-MM-dd") + ".txt";
                _fs = new FileStream(strFile, FileMode.Append, FileAccess.Write);
                _write = new StreamWriter(_fs, System.Text.Encoding.UTF8);
                    
            }
        }

        private void CloseFile()
        {
            if (_write != null)
            {
                _write.Close();
                _write = null;
            }

            if(_fs != null)
            {
                _fs.Close();
                _fs = null;
            }
        }

        private void print_log(byte logType, string str)
        {
            
            if (checkBox1.Checked && _write != null)
            {
                DateTime now = DateTime.Now;
                switch(logType)
                {                    
                    case (byte)LogType.info :
                        _write.WriteLine("[INFO]"+now.ToString("[HH-mm-ss-fff] ") + str);
                        break;

                    case (byte)LogType.err:
                        _write.WriteLine("[ERR] " + now.ToString("[HH-mm-ss-fff] ") + str);
                        break;

                    default:
                        _write.WriteLine(now.ToString("[HH-mm-ss-fff] ") + str);
                        break;
                }                
                _write.Flush();
            }
        }

        private void ListenButton_Click(object sender, EventArgs e)
        {
            if (ListenButton.Text == "Listen")
            {
                checkBox1.Enabled = false;
                textBox1.Enabled = false;
                DirSelect.Enabled = false;

                myJobManager = (CogJobManager)CogSerializer.LoadObjectFromFile(vppFilePath);

                //flush queues
                myJobManager.UserQueueFlush();
                myJobManager.FailureQueueFlush();

                // setup event handlers.  These are called when a result packet is available on
                // the User Result Queue or the Real-Time Queue, respectively.
                myJobManager.UserResultAvailable += new CogJobManager.CogUserResultAvailableEventHandler(myJobManager_UserResultAvailable);


                OpenFile();

                ConnectToServer();
                
            }
            else
            {
                CloseFile();
                checkBox1.Enabled = true;
                textBox1.Enabled = true;
                DirSelect.Enabled = true;
                StopServer();
            }
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void DirSelect_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.ShowDialog();

            textBox1.Text = dialog.SelectedPath;

        }
    }
}
