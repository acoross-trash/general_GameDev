using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MysticLabyrinth_alpha_1
{
    public partial class Form1 : Form
    {
        // 이미지 그리기
#region Battle Images
        Bitmap m_bmpEnemy;
        Bitmap[] m_bmpOut;

        int m_nBackBufSize = 2;
        int m_nCurBackBuf = 0;

        public void SwapBuffer()
        {
            m_nCurBackBuf = (m_nCurBackBuf + 1) % 2;
            pictureBox1.Image = m_bmpOut[m_nCurBackBuf];
            pictureBox1.Invalidate();
        }

        public void DrawEnemy(int idx, int nEnemyCount)
        {
            Graphics.FromImage(m_bmpOut[(m_nCurBackBuf + 1) % 2]).DrawImage(m_bmpEnemy, new Rectangle(50, 30, 90, 180));
        }

        public void ClearBackBuffer()
        {
            ClearBmp(m_bmpOut[(m_nCurBackBuf + 1) % 2]);
        }

        public void ClearBmp(Bitmap bmp)
        {
            if (bmp == null)
            {
                return;
            }

            for (int x = 0; x < bmp.Size.Width; ++x)
            {
                for (int y = 0; y < bmp.Size.Height; ++y)
                {
                    bmp.SetPixel(x, y, Color.Black);
                }
            }
        }

        void DrawScreen()
        {
            // 적 출력
            ClearBackBuffer();
            DrawEnemy(0, 1);
            SwapBuffer();
        }
#endregion

        public Form1()
        {
            InitializeComponent();
            Screen.Init(this);

            // back buffers 생성
            m_bmpOut = new Bitmap[m_nBackBufSize];
            for (int i = 0; i < m_nBackBufSize; ++i)
            {
                m_bmpOut[i] = new Bitmap(480, 240);

                ClearBmp(m_bmpOut[i]);
            }
            pictureBox1.Image = m_bmpOut[m_nCurBackBuf];
            pictureBox1.Invalidate();

            // 적 이미지 로드
            string dir = System.IO.Directory.GetCurrentDirectory();
            try
            {
                m_bmpEnemy = new Bitmap("..\\..\\..\\..\\MyResources\\Enemy_b0.png");
            }
            catch
            {
                m_bmpEnemy = null;
            }
            // end

            panel1.Visible = false;
            panel2.Visible = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //CBattleSeq cui1 = new CBattleSeq();
            //cui1.Run();

            //CUIBattleManager cui = new CUIBattleManager();
            //cui.Run();

            //Maze maze = new Maze(0, "maze1");

            DrawScreen();
        }

        #region GUI_version
       
        CNewBattle battle;

#region from_user_to_battle
        private void btGUIStart_Click(object sender, EventArgs e)
        {
            timer1.Enabled = true;
            battle = new CNewBattle(this);
            battle.Start();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lbLock)
                return;

            panel2.Visible = false;
            label2.Text = "";

            //int targetIdx = listBox1.SelectedIndex;
            int targetIdx = ((lbData)listBox1.SelectedItem).Index;
            //listBox1.Items.Clear();

            lbLock = true;

            battle.GetStateMachine().SetCommand(m_nSelectedCommand, targetIdx);
            //battle.GetStateMachine().OnTick();
        }
#endregion from_user_to_battle

#region from_battle_to_user
        
        List<string> lines;

        public void WriteLine(string line)
        {
            if (lines == null)
            {
                lines = new List<string>();
            }

            lines.Add(line);
            if (lines.Count >= 100)
            {
                lines.RemoveAt(0);
            }

            tbOutput.Text = String.Join(Environment.NewLine, lines);
            tbOutput.SelectionStart = tbOutput.Text.Length;
            tbOutput.ScrollToCaret();
        }

        List<string> linesCharParams;
        public void WriteLineToPCStateBox(string line)
        {
            if (linesCharParams == null)
            {
                linesCharParams = new List<string>();
            }

            linesCharParams.Add(line);
            if (linesCharParams.Count >= 100)
            {
                linesCharParams.RemoveAt(0);
            }

            tbCharactersParams.Text = String.Join(Environment.NewLine, linesCharParams);
            tbCharactersParams.SelectionStart = tbCharactersParams.Text.Length;
            tbCharactersParams.ScrollToCaret();
        }

        public void UpdateCharactersParams()
        {
            // 현재 전장의 상황을 출력
            if (linesCharParams == null)
            {
                linesCharParams = new List<string>();
            }
            linesCharParams.Clear();

            foreach (BattleCharacter ch in battle.GetAllCharacters())
            {
                if (ch.IsAlive())
                {
                    WriteLineToPCStateBox(string.Format("{0} : HP:{1}", ch.Name, ch.HP.ToString()));
                }
                else
                {
                    WriteLineToPCStateBox(string.Format("{0} : is DEAD", ch.Name));
                }
            }
        }

        public void ClearOutputs()
        {
            if (lines == null)
            {
                lines = new List<string>();
            }
            else
            {
                lines.Clear();
            }

            tbOutput.Text = "";
        }

        public void InitSelectCmd(string name)
        {
            // 커맨드 입력 대기 상태로.
            bGetCmdMode = true;
            panel1.Visible = true;
            
            label1.Text = name;//m_PCs[nCmdInputState].Name;
            m_CurrentCharName = name;
        }

        public void EndSelectCmd()
        {
            bGetCmdMode = false;
        }

        int m_nSelectedCommand = -1;
        bool bGetCmdMode;   // cmd 받을 수 있는 상태인지 확인
        string m_CurrentCharName;

        public void CloseBattle(int nFinishType)
        {
            panel1.Visible = false;
        }
#endregion from_battle_to_user

        public class lbData
        {
            public string Text { get; set; }
            public int Index { get; set; }
        }

        bool lbLock = true;

        // UI 내부 코드
        private void ShowTargetSelectWindow()
        {
            panel2.Visible = true;
            label2.Text = m_CurrentCharName;//m_PCs[m_nCmdInputState].Name;

            BattleCharacter[] targets;
            targets = battle.GetAllCharacters();//m_AllChars;

            List<lbData> newData = new List<lbData>();
            foreach (BattleCharacter target in targets)
            {
                if (target.IsAlive())
                {
                    newData.Add(new lbData() { Text = target.Name, Index = target.Idx });
                }
            }

            listBox1.DisplayMember = "Text";
            listBox1.DataSource = newData;

            lbLock = false;
        }

        private void btSkill_Click(object sender, EventArgs e)
        {
            if (bGetCmdMode)
            {
                m_nSelectedCommand = 1;
                ShowTargetSelectWindow();
            }
        }

        private void btAttack_Click(object sender, EventArgs e)
        {
            if (bGetCmdMode)
            {
                m_nSelectedCommand = 0;
                ShowTargetSelectWindow();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            battle.GetStateMachine().OnTick();
        }
        // UI 코드 end
        #endregion GUI_version
    }
}
