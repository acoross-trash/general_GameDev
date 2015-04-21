using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public class CUIBattleManager
    {
        CBattle m_battle;

        public CUIBattleManager()
        {
            m_battle = new CBattle();
        }

        public void Run()
        {
            m_battle.InitBattle();

            /*****************************************************/
            int nFinishType = CUIBattleLoop();

            /*****************************************************/
            // 전투 종료 처리
            CBattle.CloseBattle(nFinishType);
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
                m_battle.InitCommandList();

                // 아군의 할 일을 얻어낸다.
                m_battle.CUIGetPCCommands();

                // 적군의 할 일을 생성.
                m_battle.MakeEnemyCommands();

                // 선처리 명령을 수행한다.
                // TODO

                // 우선권을 계산한다.
                m_battle.MakeInitiList();

                m_battle.CUIExecBattleCommands(ref nFinishType, ref bFinished);
            }
            while (!bFinished);
            return nFinishType;
        }
    }
}
