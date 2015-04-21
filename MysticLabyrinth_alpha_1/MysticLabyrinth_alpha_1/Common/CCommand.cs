using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public abstract class Command
    {
        public BattleCharacter actor;

        public abstract bool Run();
    }

    public class Attack : Command
    {
        public BattleCharacter target;
        
        enum AttackState
        {
            AS_ATTACK,
            AS_ALREADY_DEAD,
            AS_DAMAGE,
            AS_KILLED
        }

        AttackState m_state;

        public Attack()
        {
            m_state = AttackState.AS_ATTACK;
        }

        public override bool Run()
        {
            // 공격자 요소: 힘, 무기공격력, 무기타입, 속성
            // 피공격자 요소: 체력, 방어구의 공격타입별방어력, 속성내성

            switch (m_state)
            {
                case AttackState.AS_ATTACK:
                    {
                        Screen.Inst().ClearScreen();
                        //Screen.Inst().WriteLine("");
                        Screen.Inst().WriteLine(String.Format("{0}이(가) {1}을(를) 공격했다!", actor.Name, target.Name));
                        if (target.IsDead())
                        {
                            m_state = AttackState.AS_ALREADY_DEAD;
                        }
                        else
                        {
                            m_state = AttackState.AS_DAMAGE;
                        }
                    }
                    return false;

                case AttackState.AS_ALREADY_DEAD:
                    {
                        Screen.Inst().WriteLine(String.Format("그러나 {0}은(는) 이미 죽어있다.", target.Name));
                    }
                    return true;

                case AttackState.AS_DAMAGE:
                    {
                        //gara
                        int nDamage = 100;
                        target.ReduceHP(nDamage);
                
                        Screen.Inst().WriteLine(String.Format("{0}은(는) {1}의 데미지를 입었다!", target.Name, nDamage));

                        if (target.IsDead())
                        {
                            m_state = AttackState.AS_KILLED;
                            return false;
                        }        
                        else
                        {
                            return true;
                        }
                    }

                case AttackState.AS_KILLED:
                    Screen.Inst().WriteLine(String.Format("{0}은(는) 죽었다.", target.Name));
                    return true;
                    
            }

            System.Diagnostics.Debug.Assert(false, "You should not be here.");
            return true;
        }
    }

    public class Defend : Command
    {
        public override bool Run()
        {
            throw new NotImplementedException();
        }
    }

    class Skill : Command
    {
        public BattleCharacter target;
        public int skillID;
        public override bool Run()
        {
            Screen.Inst().ClearScreen();
            //Screen.Inst().WriteLine("");
            Screen.Inst().WriteLine(String.Format("{0}은(는) 가만히 상황을 지켜보고 있다.", actor.Name));

            return true;
        }
    }

    class Item : Command
    {
        public BattleCharacter target;
        public int itemID;
        public override bool Run()
        {
            throw new NotImplementedException();
        }
    }

    class Escape : Command
    {
        public override bool Run()
        {
            throw new NotImplementedException();
        }
    }
}
