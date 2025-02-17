using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C03
{
    internal enum ParseIntOption
    {
        NONE = 0,
        ALLOW_WHITESPACES = 1,
        ALLOW_NEGATIVE_NUMBERS = 2,
        IGNORE_INVALID_CHARACTERS = 4,
    }
}
