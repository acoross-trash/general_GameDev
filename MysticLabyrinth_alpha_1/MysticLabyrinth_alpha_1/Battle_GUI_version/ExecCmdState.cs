using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public class ExecCommandState : BattleState
    {
        // 수행할 커맨드
        //Command[] m_CmdList;
        List<Command> m_CmdList;
        
        // 행동 우선순위 리스트
        //int[] m_InitiList;
        int m_CurrentCharIdx;
        bool IsBattleEnd;

        public ExecCommandState(/*Command[] CmdList*/ List<Command> CmdList)
        {
            m_CmdList = CmdList;
        }

        struct dt
        {
            public double initi;
            public Command Cmd;
        }

        private static int Compare(dt n1, dt n2)
        {
            if (n1.initi >= n2.initi)
                return 1;
            else
                return -1;
        }

        List<dt> m_Initiativities;
        // 우선권을 계산한다.
        void MakeInitiList(List<Command> CmdList)
        {
            m_Initiativities = new List<dt>();
            
            for (int i = 0; i < CmdList.Count; ++i)
            {
                Command c = CmdList[i];
                double initi = c.actor.GetInitiativity();

                dt node = new dt();
                node.initi = initi;
                node.Cmd = c;

                m_Initiativities.Add(node);
            }

            m_Initiativities.Sort(Compare);
        }

        // 적군의 명령을 생성한다.
        void MakeEnemiesCommands(CNewBattle battle)
        {
            for (int i = 0; i < battle.m_nEnemyNum; ++i)
            {
                Skill tmpCmd = new Skill();
                tmpCmd.actor = battle.GetEnemies()[i]; //m_Enemys[i];
                if (tmpCmd.actor.IsAlive())
                {
                    tmpCmd.target = battle.GetPCs()[i]; //m_PCs[i];
                    tmpCmd.skillID = 0; // 가만히 있기
                    //m_CmdList[battle.m_nPCNum + i] = tmpCmd;
                    m_CmdList.Add(tmpCmd);
                }
            }
        }

        public void OnStart(CNewBattle battle)
        {
            nFinishType = -1;
            battle.GetForm().CloseBattle(-1);

            IsBattleEnd = false;
            m_CurrentCharIdx = 0;
            MakeEnemiesCommands(battle);    // 적 커맨드 결정
            //MakeInitiList(battle.m_nCharNum);
            MakeInitiList(m_CmdList); // 우선권 계산

            //OnTick(battle); // 실행 -> 타이머에서 호출!!
        }

        int nFinishType;// = -1;
        public void OnTick(CNewBattle battle)
        {
            if (IsBattleEnd)
            {
                // 종료 처리
                battle.GetForm().CloseBattle(nFinishType);

                Screen.Inst().WriteLine("");
                switch (nFinishType)
                {
                    case 0: // 적 전멸
                        Screen.Inst().WriteLine("You defeated all enemies.\nBattle ends.");
                        break;
                    case 1: // 아군 전멸
                        Screen.Inst().WriteLine("You all died.\nBattle ends.");
                        break;
                    case 2: // 도망 성공
                        Screen.Inst().WriteLine("You escaped from battle.\nBattle ends.");
                        break;
                    case 3: // 이벤트
                        Screen.Inst().WriteLine("Battle ends by event.");
                        break;
                    default: // 에러
                        System.Diagnostics.Debug.Assert(false, "FinishType error.");
                        Screen.Inst().WriteLine("Battle ends. Finish Type error.");
                        break;
                }
                // 전투 종료. null state 로 이전.
                battle.GetStateMachine().ChangeState(new NullState());
                return;
            }
            else
            {
                if (m_CurrentCharIdx >= m_Initiativities.Count) // m_InitiList.Length)
                {
                    // 전투가 끝나지 않았다면 다시 커맨드를 입력받는다.
                    battle.GetStateMachine().ChangeState(new SelectCmdState());
                    return;
                }
            }

            bool bFinished = false;

            //foreach (int i in m_InitiList)  // 우선권에 따라 명령을 하나씩 수행
            //{
            Command cmd = m_Initiativities[m_CurrentCharIdx].Cmd;
                if (cmd.actor.IsAlive())
                {
                    if (cmd.Run())  // cmd tick 이 끝나면 다음 cmd 로 이전.
                    {
                        ++m_CurrentCharIdx;
                        bFinished = true;
                        battle.CheckBattleEnd(out nFinishType, out bFinished, cmd);
                    }

                    Screen.Inst().UpdateCharacterParams();
                    
                    //if (bFinished)
                    //    break;
                }
                else
                {
                    //Screen.Inst().WriteLine("");
                    //Screen.Inst().WriteLine(String.Format("{0}은(는) 이미 죽어있다.", cmd.actor.Name));
                    ++m_CurrentCharIdx;
                    OnTick(battle);
                    return;
                }
            //}

            if (bFinished)
            {
                IsBattleEnd = true;
            }
        }

        public void OnEnd(CNewBattle battle)
        {
        }

        public void SetCommand(CNewBattle battle, int nCmdNum, int targetIdx)
        {
        }
    }
}
