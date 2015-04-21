using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    class Screen
    {
        Form1 m_Form;
        static Screen inst;
        Random rand;

        public static void Init(Form1 form)
        {
            inst = new Screen();
            inst.SetForm(form);
        }

        private Screen()
        {
            rand = new Random((int)(DateTime.Now.Ticks));
        }

        void SetForm(Form1 form)
        {
            m_Form = form;
        }

        public static Screen Inst()
        {
            return inst;
        }

        public void WriteLine(string line)
        {
            m_Form.WriteLine(line);
        }

        public void ClearScreen()
        {
            m_Form.ClearOutputs();
        }

        public void UpdateCharacterParams()
        {
            m_Form.UpdateCharactersParams();
        }

        public Random Rand()
        {
            return rand;
        }
    }
}
