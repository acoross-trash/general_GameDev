using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public class CBattle
    {
        BattleCharacter[] m_PCs;
        BattleCharacter[] m_Enemys;
        BattleCharacter[] m_AllChars;
        int m_nPCNum;
        int m_nEnemyNum;
        int m_nCharNum;

        int[] m_InitiList;

        public void InitBattle()
        {
            // 전장 초기화
            m_nPCNum = 3;
            m_PCs = new BattleCharacter[m_nPCNum];
            for (int i = 0; i < m_nPCNum; ++i)
            {
                BattleCharacter tmp = new BattleCharacter();
                tmp.MaxHP = 100;
                tmp.HP = 100;
                tmp.Idx = i;
                m_PCs[i] = tmp;
            }
            m_PCs[0].Name = "Warrior";
            m_PCs[1].Name = "Magician";
            m_PCs[2].Name = "Cleric";

            m_nEnemyNum = 3;
            m_Enemys = new BattleCharacter[m_nEnemyNum];
            for (int i = 0; i < m_nEnemyNum; ++i)
            {
                BattleCharacter tmp = new BattleCharacter();
                tmp.Name = "Enemy" + i.ToString();
                tmp.MaxHP = 100;
                tmp.HP = 100;
                tmp.Idx = i + m_nPCNum;
                m_Enemys[i] = tmp;
            }

            m_nCharNum = m_nPCNum + m_nEnemyNum;
            m_AllChars = new BattleCharacter[m_nCharNum];
            for (int i = 0; i < m_nPCNum; ++i)
                m_AllChars[i] = m_PCs[i];
            for (int i = 0; i < m_nEnemyNum; ++i)
                m_AllChars[m_nPCNum + i] = m_Enemys[i];

            // 전투 시작 메시지
            Console.WriteLine("Battle Start!");
        }

        public static void CloseBattle(int nFinishType)
        {
            switch (nFinishType)
            {
                case 0: // 적 전멸
                    Console.WriteLine("You defeated all enemies.\nBattle ends.");
                    break;
                case 1: // 아군 전멸
                    Console.WriteLine("You all died.\nBattle ends.");
                    break;
                case 2: // 도망 성공
                    Console.WriteLine("You escaped from battle.\nBattle ends.");
                    break;
                case 3: // 이벤트
                    Console.WriteLine("Battle ends by event.");
                    break;
                default: // 에러
                    System.Diagnostics.Debug.Assert(false, "FinishType error.");
                    Console.WriteLine("Battle ends. Finish Type error.");
                    break;
            }
        }

        #region Commands
        
        Command[] m_CmdList;

        public int GetCommandState()
        {
            return m_nGetCommandState;
        }

        public void SetCommandState(int nState)
        {
            m_nGetCommandState = nState;
        }

        public void InitCommandList()
        {
            SetCommandState(0);
            m_CmdList = new Command[m_nCharNum];
        }

        public void SetCommandIter(int cmd)
        {
            SetCommand(m_nGetCommandState, cmd);
            m_nGetCommandState++;
        }

        public void SetCommand(int _i, int cmd)
        {
            BattleCharacter PC = m_PCs[_i];
            switch (cmd)
            {
                case 0:
                    {
                        Attack tmpCmd = new Attack();
                        tmpCmd.actor = PC;
                        tmpCmd.target = m_Enemys[_i];
                        m_CmdList[_i] = tmpCmd;
                        break;
                    }
                default:
                    {
                        Skill tmpCmd = new Skill();
                        tmpCmd.actor = PC;
                        tmpCmd.target = PC;
                        tmpCmd.skillID = 0; // 가만히 있기
                        m_CmdList[_i] = tmpCmd;
                        break;
                    }
            }
        }

        public void CUIExecBattleCommands(ref int nFinishType, ref bool bFinished)
        {
            // 우선권에 따라 명령을 수행한다.
            foreach (int i in m_InitiList)
            {
                Command cmd = m_CmdList[i];
                if (cmd.actor.IsAlive())
                {
                    cmd.Run();

                    CheckBattleEnd(ref nFinishType, ref bFinished, cmd);
                }

                if (bFinished)
                    break;
            }
        }
        //GARA
        public void MakeEnemyCommands()
        {
            // 적군의 명령을 생성한다.
            for (int i = 0; i < m_nEnemyNum; ++i)
            {
                Skill tmpCmd = new Skill();
                tmpCmd.actor = m_Enemys[i];
                tmpCmd.target = m_PCs[i];
                tmpCmd.skillID = 0; // 가만히 있기
                m_CmdList[m_nPCNum + i] = tmpCmd;
            }
        }

        public void CUIGetPCCommands()
        {
            for (int i = 0; i < m_nPCNum; ++i)
            {
                BattleCharacter PC = m_PCs[i];
                string line;
                int num = -1;
                do
                {
                    Console.WriteLine("{0}은(는) 무엇을 합니까?", PC.Name);
                    line = Console.ReadLine();
                }
                while (false == int.TryParse(line, out num));

                SetCommand(i, num);
            }
        }

        int m_nGetCommandState;

        #endregion Commands


        public void MakeInitiList()
        {
            m_InitiList = new int[m_nCharNum];
            for (int i = 0; i < m_nCharNum; ++i)
            {
                m_InitiList[i] = i;
            }
        }

        private void CheckBattleEnd(ref int nFinishType, ref bool bFinished, Command cmd)
        {
            // 아군 전멸?
            bool bAllDead = true;
            foreach (BattleCharacter pc in m_PCs)
            {
                if (pc.IsAlive())
                {
                    bAllDead = false;
                    break;
                }
            }
            if (bAllDead)
            {
                nFinishType = 1;
                bFinished = true;
            }
            else
            {
                // 적군 전멸?
                bAllDead = true;
                foreach (BattleCharacter enemy in m_Enemys)
                {
                    if (enemy.IsAlive())
                    {
                        bAllDead = false;
                        break;
                    }
                }
                if (bAllDead)
                {
                    nFinishType = 0;
                    bFinished = true;
                }

                // 도망 성공?
                // todo
            }
        }
    }
}
