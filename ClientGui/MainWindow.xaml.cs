/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Prototype for OOD Project #4               //
// ver 1.4                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
// Modified by   : Yue Liu , Syracuse University                   //
//                 315-278-7363, yliu241@syr.edu                   //                
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class MainWindow that provides tabs:
 * - Find Libs: Navigate through remote server directory to find files for testing
 *   - Shows remote server directories and files
 *   - Can navigate by double clicking directories
 *   - Can select files by selecting.  That displays selections in a popup window.
 * - Request Tests(GUI direction): When you first open client interface, the back-end code
 *                                 will automatically send command to server to navigate, 
 *                                 convert and download files in Server. Then the server will
 *                                 reply a set of message to client. Client can use these message 
 *                                 to display relative information on GUI interface.
 *                                 Client GUI also provide some button to help you make some operations.
 *                                 1. the REFRESH button is to re-send command to server with new regex 
 *                                 you defined in GUI interface.
 *                                 2. the SEND button is to download converted HTML files that stored in
 *                                 Server Dir.
 * - Show Results:  left for students
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.4 : 30 April 2019
 * - Modified the some GUI interface and function in order to fit requirements of PJ#4
 * ver 1.3 : 7 April 2019
 * - added some new method
 * ver 1.2 : 28 Mar 2019
 * - fixed bug in DirsMouse_DoubleClick by using try catch block to
 *   handle exception that occurs if user clicks on unpopulated listbox item.
 * ver 1.1 : 16 Nov 2018
 * - fixed bug in Files_SelectionChanged by checking e.AddedItems.Count
 *   and returning if 0.
 * ver 1.0 : 15 Nov 2018
 * - first release
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Text.RegularExpressions;
using System.ComponentModel;
using System.Threading;
using Microsoft.Win32;
using MsgPassingCommunication;


namespace ClientGui
{
    ///////////////////////////////////////////////////////////////////
    // MainWindow class
    // - Provides navigation and selection to find libraries to test.
    // - Creates a popup window to handle selections.

    public partial class MainWindow : Window
    {
        public string path { get; set; }
        public string[] cmd {get; set;}
        public string path_web { get; set; }
        public string root_path { get; set; }
        public string suffix1 { get; set; }
        public string suffix2 { get; set; }
        List<string> needFiles { get; set; } = new List<string>();
        List<string> selectedFiles { get; set; } = new List<string>();
        public SelectionWindow swin { get; set; } = null;
        bool unselecting = false;
        private System.Windows.Threading.DispatcherTimer time1 = new System.Windows.Threading.DispatcherTimer();
        private System.Windows.Threading.DispatcherTimer time2 = new System.Windows.Threading.DispatcherTimer();
        public MainWindow()
        {
            InitializeComponent();
            time1.Tick += new EventHandler(autoexe1);
            time1.Interval = new TimeSpan(0, 0, 0, 5, 0);
            time1.Start();
        }
        
        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();
        //----< this function is to automatically execute some requirements in PJ#4 >----------------
        private void autoexe1(object sender, EventArgs e)
        {
            tab1.SelectedItem = tab1.Items[1];
            htmlbutton.RaiseEvent(new RoutedEventArgs(System.Windows.Controls.Button.ClickEvent));
            if (ConvertedFiles.Items.Count > 0)
            {
                string selectedFile = ConvertedFiles.Items[0].ToString();
                selectedFile = System.IO.Path.GetFileName(selectedFile);
                selectedFile = System.IO.Path.Combine(path_web, selectedFile);
                if (swin == null)
                {
                    swin = new SelectionWindow();
                    swin.setMainWindow(this);
                }
                StreamReader sr = new StreamReader(selectedFile, Encoding.Default);
                string content;
                while ((content = sr.ReadLine()) != null)
                {
                    swin.Add(content.ToString());
                }
                swin.Show();
                SetWebBrowserFeatures(11);
                WebBrowser1.Navigate(new Uri(selectedFile));
            }
            time1.Stop();
            time2.Tick += new EventHandler(autoexe2);
            time2.Interval = new TimeSpan(0, 0, 0, 2, 0);
            time2.Start();
        }
        //----< this function is to automatically execute some requirements in PJ#4 >----------------
        private void autoexe2(object sender, EventArgs e)
        {
            tab1.SelectedItem = tab1.Items[2];
            time2.Stop();
        }
        //----< process cmd line >----------------
        private void processcmd(string [] cmd)
        {
            root_path = cmd[1];
            suffix1 = cmd[4];
            suffix2 = cmd[5];
        }
        //----< process incoming messages on child thread >----------------
        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    if (msg.attributes.Count == 0) continue;
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }       
        //----< function dispatched by child thread to main thread >-------
        private void clearDirs()
        {
            Dirs.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------
        private void addpath(string path)
        {
            CurrPath.Text = path;
        }
        //----< function dispatched by child thread to main thread >-------

        private void addDir(string dir)
        {
            Dirs.Items.Add(dir);
        }
        //----< function dispatched by child thread to main thread >-------

        private void insertParent()
        {
            Dirs.Items.Insert(0, "..");
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearFiles()
        {
            Files.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------
        private void addFile(string file)
        {
            Files.Items.Add(file);
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearConvertFiles()
        {
            RemoteConvertedFiles.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addConvertedFile(string file)
        {
            RemoteConvertedFiles.Items.Add(file);
        }
        //----< add client processing for message with key >---------------
        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }
        //----< load getDirs processing into dispatcher dictionary >-------      
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                    else if(key.Contains("path"))
                    {
                        Action<string> dopath = (string path) =>
                        {
                            addpath(path);
                        };
                        Dispatcher.Invoke(dopath, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }
        //----< load getFiles processing into dispatcher dictionary >------
        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };        
            addClientProc("getFiles", getFiles);
        }
        //----< load convertFiles processing into dispatcher dictionary >------
        private void DispatcherConvertedFiles()
        {
            Action<CsMessage> convertFiles = (CsMessage rcvMsg) =>
            {
                Action clrConvertFiles = () =>
                {
                    clearConvertFiles();
                };
                Dispatcher.Invoke(clrConvertFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("ConvertedFiles"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addConvertedFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("convertFiles", convertFiles);
        }
        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherConvertedFiles();
        }
        //----< the function that sends convert files command  >---------------------------
        private void convertfiles()
        {
            string commandline = cmd[0];
            foreach (var a in cmd)
            {
                if (!commandline.Contains(a))
                    commandline = commandline + " " + a;
            }
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "convertFiles");
            msg.add("cmd", commandline);
            translater.postMessage(msg);
        }
        //----< the function that sends download files command  >---------------------------
        private void sendfiles()
        {
            int size = RemoteConvertedFiles.Items.Count;
            int i = 0;
            while (i < size)
            {
                string filename = RemoteConvertedFiles.Items[i].ToString() + ".html";
                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                serverEndPoint.port = 8080;

                pathStack_.Push("../RemoteConvertedWebpages");
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "sendfiles");
                msg.add("filename", filename);
                msg.add("path", pathStack_.Peek());
                translater.postMessage(msg);
                i++;
                System.Threading.Thread.Sleep(500);
            }
        }
        //----< the function that sends download files command  >---------------------------
        private void sendfiles(string filename)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;

            pathStack_.Push("../RemoteConvertedWebpages");
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "sendfiles");
            msg.add("filename", filename);
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            if (!ConvertedFiles.Items.Contains(filename+".html"))
                ConvertedFiles.Items.Add(filename + ".html");
        }
        //----< open window showing contents of project directory >------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            cmd = Environment.GetCommandLineArgs();
            processcmd(cmd);
            path = Directory.GetCurrentDirectory();
            path = getAncestorPath(3, path);
            path_web = path + "\\LocalConvertedWebpages";
            Loadcmd(cmd);                    
            endPoint_ = new CsEndPoint();            // start Comm
            endPoint_.machineAddress = "localhost";
            endPoint_.port = 8082;
            translater = new Translater();
            translater.listen(endPoint_);
            processMessages();          // start processing messages
            loadDispatcher();           // load dispatcher
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CurrPath.Text = "Storage";
            pathStack_.Push("..");
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            msg.add("suffix1", suffix1);
            msg.add("suffix2", suffix2);
            translater.postMessage(msg);
            convertfiles();
        }
        //----< find parent paths >--------------------------------------
        string getAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }
        //----< show converted web files in Convert Files interface >-------
        void LoadWebFile(string path)
        {
            if (ConvertedFiles.Items.Count > 0)
            ConvertedFiles.Items.Clear();
            string[] files = Directory.GetFiles(path);
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                ConvertedFiles.Items.Add(name);
            }
        }
        //----< load default cmd >-------
        void Loadcmd(string [] argv)
        {
            string s = string.Empty;
            for (int i = 1; i < argv.Length - 1; i++)
            {
                s += argv[i] + " ";
            }
            CMD.Text = s;
            string a = argv[argv.Length - 1];
            regex.Text = a;
        }
        //----< unselect files called by unloading SelectionWindow >-----
        public void unselectFiles()
        {
            unselecting = true;  // needed to avoid using selection logic
                                 //selectedFiles.Clear();
            Files.UnselectAll();
        }
        //----< strip off name of first part of path >---------------------
        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("\"");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }
        //----< move into double-clicked directory, display contents >---
        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)Dirs.SelectedItem;
            if (selectedDir == null)
                return;
            string path;
            if (selectedDir == "..")
            {
                if (pathStack_.Count > 1)  // don't pop off "Storage"
                    pathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = pathStack_.Peek() + "/" + selectedDir;
                pathStack_.Push(path);
            }
            CurrPath.Text = removeFirstDir(pathStack_.Peek());             // display path in Dir TextBlcok
            CsEndPoint serverEndPoint = new CsEndPoint();          // build message to get dirs and post it
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFiles");
            msg.add("suffix1", suffix1);
            msg.add("suffix2", suffix2);
            translater.postMessage(msg);
        }
        //----< open target html file in pop-up window with html source code and in browse result with web page >---
        private void HTML_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedFile = (string)ConvertedFiles.SelectedItem;
            selectedFile = System.IO.Path.Combine(path_web, selectedFile);
            if (unselecting)
            {
                unselecting = false;
                return;
            }
            if (swin == null)
            {
                swin = new SelectionWindow();
                swin.setMainWindow(this);
            }
            StreamReader sr = new StreamReader(selectedFile, Encoding.Default);
            string content;
            while ((content = sr.ReadLine()) != null)
            {
                swin.Add(content.ToString());
            }
            swin.Show();
            SetWebBrowserFeatures(11);
            WebBrowser1.Navigate(new Uri(selectedFile));
            
        }
        //----< download one file from server >----------------   
        private void SendOneFile_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedFile = (string)RemoteConvertedFiles.SelectedItem;
            sendfiles(selectedFile);
        }
        //----< refresh the system with new regex >-------       
        private void Refresh(object sender, RoutedEventArgs e)
        {
            string b = regex.Text;
            cmd[6] = b;
            convertfiles();
            sendfiles();
        }
        //----< download file click from server >---------------------
        private void Send(object sender, RoutedEventArgs e)
        {
            int size = RemoteConvertedFiles.Items.Count;
            int i = 0;
            while(i < size)
            {
                string filename = RemoteConvertedFiles.Items[i].ToString() + ".html";

                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                serverEndPoint.port = 8080;

                pathStack_.Push("../RemoteConvertedWebpages");
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "sendfiles");
                msg.add("filename", filename);
                msg.add("path", pathStack_.Peek());
                translater.postMessage(msg);
                System.Threading.Thread.Sleep(500);
                i++;
            }
            LoadWebFile(path_web);
        }
        //----< change the IE kernel >-------
        static void SetWebBrowserFeatures(int ieVersion)
        {
            if (LicenseManager.UsageMode != LicenseUsageMode.Runtime)
                return;
            var appName = System.IO.Path.GetFileName(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName);
            UInt32 ieMode = GeoEmulationModee(ieVersion);
            var featureControlRegKey = @"HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\Main\FeatureControl\";
            Registry.SetValue(featureControlRegKey + "FEATURE_BROWSER_EMULATION",
                appName, ieMode, RegistryValueKind.DWord);
            Registry.SetValue(featureControlRegKey + "FEATURE_ENABLE_CLIPCHILDREN_OPTIMIZATION",
                appName, 1, RegistryValueKind.DWord);
        }

        static UInt32 GeoEmulationModee(int browserVersion)
        {
            UInt32 mode = 11000; // Internet Explorer 11. Webpages containing standards-based !DOCTYPE directives are displayed in IE11 Standards mode. 
            switch (browserVersion)
            {
                case 7:
                    mode = 7000; // Webpages containing standards-based !DOCTYPE directives are displayed in IE7 Standards mode. 
                    break;
                case 8:
                    mode = 8000; // Webpages containing standards-based !DOCTYPE directives are displayed in IE8 mode. 
                    break;
                case 9:
                    mode = 9000; // Internet Explorer 9. Webpages containing standards-based !DOCTYPE directives are displayed in IE9 mode.                    
                    break;
                case 10:
                    mode = 10000; // Internet Explorer 10.
                    break;
                case 11:
                    mode = 11000; // Internet Explorer 11
                    break;
            }
            return mode;
        }
        //----< shut down the SelectionWindow if open >------------------
        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            swin.Close();
        }
    }
}
