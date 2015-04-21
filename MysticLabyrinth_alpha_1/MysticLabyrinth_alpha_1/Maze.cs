using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticLabyrinth_alpha_1
{
    public class Maze
    {
        // PCs coord
        int m_X;
        int m_Y;

        int m_nIndex;
        string m_sName;

        public Maze(int m_nIndex, string m_sName)
        {
            System.IO.FileStream fs = System.IO.File.Open("c:\\temp\\maze1.txt", System.IO.FileMode.Open);
            
            int b = 0;
            if (fs.Length < 2)
            {
                return;
            }

            m_X = fs.ReadByte();
            m_Y = fs.ReadByte();

            for (int i = 2; i < fs.Length; ++i)
            {
                b = fs.ReadByte();
            }
        }
    }
}
