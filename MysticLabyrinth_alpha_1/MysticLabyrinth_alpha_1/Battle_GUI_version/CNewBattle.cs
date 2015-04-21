using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public class CNewBattle
    {
        // View
        Form1 m_form;
        public Form1 GetForm()
        {
            return m_form;
        }

        // Data
        BattleCharacter[] m_PCs;
        public int m_nPCNum;

        BattleCharacter[] m_Enemys;
        public int m_nEnemyNum;

        BattleCharacter[] m_AllChars;
        public int m_nCharNum;

        // State machine
        StateMachine m_StateMachine;

        public StateMachine GetStateMachine()
        {
            return m_StateMachine;
        }

        // view and state
        public CNewBattle(Form1 form)
        {
            m_form = form;
            //m_CurrentState = new InitState();
            m_StateMachine = new StateMachine(this);
        }

        public void Start()
        {
            m_StateMachine.Run();
        }

        // data
        public void InitBattle()
        {
            Screen.Inst().ClearScreen();

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
            Screen.Inst().WriteLine("Battle Start!");
        }

        // data
        public BattleCharacter[] GetPCs()
        {
            return m_PCs;
        }

        public BattleCharacter[] GetEnemies()
        {
            return m_Enemys;
        }

        public BattleCharacter[] GetAllCharacters()
        {
            return m_AllChars;
        }

        public void CheckBattleEnd(out int nFinishType, out bool bFinished, Command cmd)
        {
            // 아군 전멸?
            bFinished = false;
            nFinishType = -1;
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
