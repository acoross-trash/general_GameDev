using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public class InitState : BattleState
    {
        public void OnStart(CNewBattle battle)
        {
            battle.InitBattle();
            battle.GetStateMachine().ChangeState(new SelectCmdState());
        }

        public void OnTick(CNewBattle battle)
        {
        }

        public void OnEnd(CNewBattle battle)
        {
            ;
        }

        public void SetCommand(CNewBattle battle, int nCmdNum, int targetIdx)
        {
            ;
        }
    }
}
