using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public interface BattleState
    {
        void OnStart(CNewBattle battle);
        void OnTick(CNewBattle battle);
        void OnEnd(CNewBattle battle);

        void SetCommand(CNewBattle battle, int nCmdNum, int targetIdx);
    }

    public class StateMachine
    {
        // battle state 코드
        #region batte states
        BattleState m_CurrentState;
        CNewBattle m_Owner;

        public StateMachine(CNewBattle owner)
        {
            m_Owner = owner;
            m_CurrentState = new InitState();
        }

        public void ChangeState(BattleState newState)
        {
            m_CurrentState.OnEnd(m_Owner);
            m_CurrentState = newState;
            m_CurrentState.OnStart(m_Owner);
        }

        public void Run()
        {
            m_CurrentState.OnStart(m_Owner);
            m_CurrentState.OnTick(m_Owner);
        }

        public BattleState GetState()
        {
            return m_CurrentState;
        }

        public void SetCommand(int nCmd, int targetIdx)
        {
            m_CurrentState.SetCommand(m_Owner, nCmd, targetIdx);
        }

        public void OnTick()
        {
            m_CurrentState.OnTick(m_Owner);
        }
        #endregion battle states
        // battle State end
    }
}
