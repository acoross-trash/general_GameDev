using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public class BattleCharacter
    {
        public string Name;
        public int MaxHP;
        public int HP;

        public int Idx;

        public bool IsDead()
        {
            return !IsAlive();
        }

        public bool IsAlive()
        {
            return HP > 0;
        }

        public int ReduceHP(int nDamage)
        {
            HP -= nDamage;

            if (HP < 0)
                HP = 0;

            return HP;
        }

        public double GetInitiativity()
        {
            return Screen.Inst().Rand().NextDouble();
        }
    }
}
