using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public class SelectCmdState : BattleState
    {
        int m_nCmdInputState = 0;   // 입력받으려는 pc 의 index

        // 수행할 커맨드
        //Command[] m_CmdList;
        List<Command> m_CmdList;

#region Interfaces
        public void OnStart(CNewBattle battle)
        {
            m_nCmdInputState = -1;

            //m_CmdList = new Command[battle.m_nCharNum];
            m_CmdList = new List<Command>();

            BattleCharacter[] pcs = battle.GetPCs();

            Screen.Inst().UpdateCharacterParams();

            GetCmdOfNextPc(battle);
            //ToNextPC(battle);
            //battle.GetForm().InitSelectCmd(battle.GetPCs()[m_nCmdInputState].Name);
        }

        public void OnTick(CNewBattle battle)
        {
            //ToNextPC(battle);
        }

        public void OnEnd(CNewBattle battle)
        {
            //bGetCmdMode = false;
            battle.GetForm().EndSelectCmd();
        }

        public void SetCommand(CNewBattle battle, int nCmdNum, int targetIdx)
        {
            _SetCommand(battle, nCmdNum, targetIdx);
            GetCmdOfNextPc(battle);
        }
#endregion Interfaces

        void GetCmdOfNextPc(CNewBattle battle)
        {
            BattleCharacter[] pcs = battle.GetPCs();

            ++m_nCmdInputState;
            if (m_nCmdInputState >= battle.m_nPCNum)
            {
                //// 명령 수행 상태로.
                battle.GetStateMachine().ChangeState(new ExecCommandState(m_CmdList));
            }
            else
            {
                if (pcs[m_nCmdInputState].IsDead())
                {
                    //_SetCommand(battle, 0, 0);    // List 로 바꾸었기 때문에, 필요 없어짐.
                    GetCmdOfNextPc(battle);
                }
                else
                {
                    battle.GetForm().InitSelectCmd(battle.GetPCs()[m_nCmdInputState].Name);
                }
            }
        }

        void _SetCommand(CNewBattle battle, int nCmdNum, int targetIdx)
        {
            BattleCharacter PC = battle.GetPCs()[m_nCmdInputState]; //m_PCs[m_nCmdInputState];
            switch (nCmdNum)
            {
                case 0:
                    {
                        Attack tmpCmd = new Attack();
                        tmpCmd.actor = PC;
                        tmpCmd.target = battle.GetAllCharacters()[targetIdx]; //m_AllChars[targetIdx];
                        //m_CmdList[m_nCmdInputState] = tmpCmd;
                        m_CmdList.Add(tmpCmd);
                        break;
                    }
                default:
                    {
                        Skill tmpCmd = new Skill();
                        tmpCmd.actor = PC;
                        tmpCmd.target = PC;
                        tmpCmd.skillID = 0; // 가만히 있기
                        //m_CmdList[m_nCmdInputState] = tmpCmd;
                        m_CmdList.Add(tmpCmd);
                        break;
                    }
            }
        }

        //void ToNextPC(CNewBattle battle)
        //{
        //    BattleCharacter[] pcs = battle.GetPCs();
        //    for (++m_nCmdInputState; m_nCmdInputState < battle.m_nPCNum; ++m_nCmdInputState)
        //    {
        //        if (pcs[m_nCmdInputState].IsAlive())
        //        {
        //            break;
        //        }
        //        else
        //        {
        //            _SetCommand(battle, 0, 0);
        //        }
        //    }

        //    if (m_nCmdInputState >= battle.m_nPCNum)
        //    {
        //        //// 명령 수행 상태로.
        //        battle.GetStateMachine().ChangeState(new ExecCommandState(m_CmdList));
        //    }
        //    else
        //    {
        //        // 다음 캐릭터 cmd 받기.
        //        //label1.Text = m_PCs[m_nCmdInputState].Name;
        //        battle.GetForm().InitSelectCmd(battle.GetPCs()[m_nCmdInputState].Name);
        //    }
        //}
    }

}
