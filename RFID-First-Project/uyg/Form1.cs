using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Threading;

struct member
{
    public string name;   
    public bool inside;
    public DateTime exit_time;
    public DateTime enter_time;
};

namespace uyg
{
    
    public partial class Form1 : Form
    { 
        string datetimeString;
        string currentLoggingPath1 = @"C:\Users\AESK-Mekanik\Desktop\kayit\firstDateTxt.txt";
        string currentLoggingPath = @"C:\Users\AESK-Mekanik\Desktop\kayit\currentLog.txt";

        string[] portlar = SerialPort.GetPortNames();
        //string[] base_lines = System.IO.File.ReadAllLines(@"F:\Yusuf\Ders & Bölüm\AESK\cobluk\bos.txt");
        //string currentLoggingPath = @"F:\Yusuf\Ders & Bölüm\AESK\cobluk\currentLog.txt";
        //string log_adress = @"F:\Yusuf\Ders & Bölüm\AESK\cobluk\bos.txt";

        member[] members = new member[500];

        int m = 0;
        
        string tempName;

        public Form1()
        {
            InitializeComponent();
        }

        
        private void button2_Click(object sender, EventArgs e) //şimdilik başlat butonu
        {
          
        }

        private void Form1_Load(object sender, EventArgs e)
        {
           // timer1.Start();
            foreach (string port in portlar)
            {                
                comboBox1.Items.Add(port);
                comboBox1.SelectedIndex = 0;               
            }
            label3.ForeColor = Color.Red;
            label3.Text = "Bağlantı Kapalı";
            pictureBox1.BackColor = Color.Transparent;
            label1.BackColor = Color.Transparent;
            //label2.BackColor = Color.Transparent;
            label3.BackColor = Color.Transparent;
            label5.BackColor = Color.Transparent;
            //label4.BackColor = Color.Transparent;
            FormBorderStyle = FormBorderStyle.None;
            WindowState = FormWindowState.Maximized;
            button1.BackColor = Color.Transparent;
            Control.CheckForIllegalCrossThreadCalls = false;
            // label2.Text = ("Değer");
        }

 

        public static void log(string filePath, string text)
        {
                using (StreamWriter loglamatxt = File.AppendText(filePath))
                {
                    loglamatxt.Write(text);
                }

        }

        public static void makeNewTxt(string log_address)
        {
            using (FileStream fs = File.Create(log_address))
            {

            }
        }


private void button4_Click(object sender, EventArgs e)
        {
            try
            {
                if (serialPort1.IsOpen == false)
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.BaudRate = 9600;
                    serialPort1.Open();
                    label3.ForeColor = Color.Green;
                    label3.Text = "Bağlantı Açık";
                    serialPort1.DataReceived += new SerialDataReceivedEventHandler(serialPort1_DataReceived);

                }
            }
            catch (Exception hata)
            {
                MessageBox.Show("Hata:" + hata.Message);
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {
            
        }

        private void button3_Click(object sender, EventArgs e)
        {
           


        }

        private void button1_Click(object sender, EventArgs e)
        {      
            bool is_register_able = true;
            
            char[] newForm = new char[12];
            
            for (int i = 0; i < label2.Text.Length - 1; i++)
            {
                newForm[i] = label2.Text[i];
            }
            for (int i = 11; i > label2.Text.Length - 2; i--)
            {
                newForm[i] = '#';
            }
            
            string newForm2 = new string(newForm);
            foreach(string idRead in File.ReadLines(@"C:\Users\AESK-Mekanik\Desktop\kayit\idregister.txt"))
            {
                if(idRead == newForm2)
                {
                    is_register_able = false;
                }
            }
            

            if (is_register_able == true)
            {
                using (StreamWriter datatxt = File.AppendText(@"C:\Users\AESK-Mekanik\Desktop\kayit\idregister.txt"))
                {
                    datatxt.Write("\n" + textBox1.Text + "\n"); //textboxtan alınan veri texte yollanacak
                    datatxt.Write(newForm2);
                    MessageBox.Show("Başarıyla kaydedildi! Tamama basmadan kart okutmayınız!");
                }
            }
            else
            {
                MessageBox.Show("Zaten Kayıtlı");
            }

            textBox1.Text = "";
            
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen == true)
            {
                serialPort1.Close();
                label3.ForeColor = Color.Red;
                label3.Text = "Bağlantı Kapalı";
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            
            if ((DateTime.Now.Hour == 0))
            {
                datetimeString = string.Format("{0:yyyy-MM-dd hh:mm}.txt", DateTime.Now);
                currentLoggingPath1 = @"C:\Users\AESK-Mekanik\Desktop\kayit\" + datetimeString;
                if (!File.Exists(currentLoggingPath1))
                {
                    makeNewTxt(currentLoggingPath1);
                }
                return;
            }
        }       

        private void button7_Click(object sender, EventArgs e)
        {
            
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (serialPort1.IsOpen == true)
            {


                string gelen = serialPort1.ReadLine();

                label2.Text = gelen;
                //listBox1.Items.Add(label2.Text);
                
                char[] newForm = new char[12];

                for (int i = 0; i < label2.Text.Length - 1; i++)
                {
                    newForm[i] = label2.Text[i];
                }
                for (int i = 11; i > label2.Text.Length - 2; i--)
                {
                    newForm[i] = '#';
                }

                string newForm2 = new string(newForm);

                m = 0;
                // ID register taranıyor
                

                foreach (string idRead in File.ReadLines(@"C:\Users\AESK-Mekanik\Desktop\kayit\idregister.txt"))
                {
                    if (idRead == newForm2)
                    {
                        //Giriş
                        if (members[m].inside == false) //members[m].inside txt read ile yapılacak 
                        {
                            members[m].inside = true;
                            members[m].name = tempName;
                            members[m].enter_time = DateTime.Now;
                            log(currentLoggingPath, (members[m].name + "     Giriş Zamanı: " + (members[m].enter_time) + "\n"));
                            listBox1.Items.Add(members[m].name);
                            listBox1.SetSelected(listBox1.Items.Count - 1, true);
                            label4.Text = "Son Giriş:" + members[m].name;
                            // MessageBox.Show("giris");
                            break;
                        }
                        //Çıkış
                        else if (members[m].inside == true)
                        {
                            members[m].inside = false;
                            members[m].name = tempName;
                            members[m].exit_time = DateTime.Now;
                            log(currentLoggingPath, (members[m].name + "     Çıkış Zamanı: " + (members[m].exit_time) + "\n"));
                            log(currentLoggingPath1, members[m].name + "$" + (members[m].enter_time) + "$" + (members[m].exit_time) + "\n");
                            listBox2.Items.Add(members[m].name + "     Giriş Zamanı: " + (members[m].enter_time) + "     Çıkış Zamanı: " + (members[m].exit_time));
                            listBox2.SetSelected(listBox2.Items.Count - 1, true);
                            //AF CİKİSA LOGLAYACAK
                            //appendleme                      
                            //MessageBox.Show("cikis");
                            break;
                        }
                    }
                    tempName = idRead;
                    m++;
                }
                //timer1.Start();
            }
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void listBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void buton2_Click(object sender, EventArgs e)
        {
            this.Close();
            Application.Exit();
        }

    }
    
}
