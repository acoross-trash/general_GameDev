using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    class CBattleSeq
    {
        BattleCharacter[] m_PCs;
        int m_nPCNum;

        BattleCharacter[] m_Enemys;
        int m_nEnemyNum;

        BattleCharacter[] m_AllChars;
        int m_nCharNum;

        // 수행할 커맨드
        Command[] m_CmdList;

        // 행동 우선순위 리스트
        int[] m_InitiList;

        public void Run()
        {
            InitBattle();

            /*****************************************************/
            int nFinishType = CUIBattleLoop();

            /*****************************************************/
            // 전투 종료 처리
            CloseBattle(nFinishType);
        }

        private int CUIBattleLoop()
        {
            // 전투 루프
            // 종료타입 nFinishType: 0. 적 전멸 1. 아군 전멸 2. 도망 성공 3. 이벤트
            int nFinishType = -1;
            bool bFinished = false;
            do
            {
                //command: 0.공격, 1.스킬, 2.방어, 3.도망, 4.아이템
                m_CmdList = new Command[m_nCharNum];

                // 아군의 할 일을 얻어낸다.
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

                // 적군의 명령을 생성한다.
                for (int i = 0; i < m_nEnemyNum; ++i)
                {
                    Skill tmpCmd = new Skill();
                    tmpCmd.actor = m_Enemys[i];
                    tmpCmd.target = m_PCs[i];
                    tmpCmd.skillID = 0; // 가만히 있기
                    m_CmdList[m_nPCNum + i] = tmpCmd;
                }

                // 선처리 명령을 수행한다.
                // TODO

                // 우선권을 계산한다.
                m_InitiList = new int[m_nCharNum];
                for (int i = 0; i < m_nCharNum; ++i)
                {
                    m_InitiList[i] = i;
                }

                CUIExecBattleCommands(ref nFinishType, ref bFinished);
            }
            while (!bFinished);
            return nFinishType;
        }

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

        private void SetCommand(int _i, int cmd)
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

        private void CUIExecBattleCommands(ref int nFinishType, ref bool bFinished)
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
