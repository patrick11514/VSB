using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C03
{
      internal class IntParser
    {
        public static int ParseInt(string txt)
        {
            int result = 0;

            for (int i = 0; i < txt.Length; i++)
            {
                char c = txt[i];

                int tmp = c - '0';

                if (tmp >= 0 && tmp <= 9)
                {
                    result *= 10;
                    result += tmp;
                } else
                {
                    return -1;
                }

            }

            return result;
        }
        public static int? ParseIntOrNull(string txt)
        {
            int result = 0;

            for (int i = 0; i < txt.Length; i++)
            {
                char c = txt[i];

                int tmp = c - '0';

                if (tmp >= 0 && tmp <= 9)
                {
                    result *= 10;
                    result += tmp;
                }
                else
                {
                    return null;
                }

            }

            return result;
        }

        public static bool TryParseInt(string txt, out int result)
        {
            int? num = ParseIntOrNull(txt);
            if (num != null)
            {
                result = num.Value;
                return true;
            }
            result = 0;
            return false;
        }

        public static bool TryParseInt(string txt, ParseIntOption option, out int result)
        {
            result = 0;

            bool minus = false;

            for (int i = 0; i < txt.Length; i++)
            {
                char c = txt[i];

                if (option.HasFlag(ParseIntOption.ALLOW_WHITESPACES) && char.IsWhiteSpace(c)) continue;
                if (
                    option.HasFlag(ParseIntOption.ALLOW_NEGATIVE_NUMBERS) && 
                    c == '-' && 
                    i == 0
                    )
                {
                    minus = true;
                    continue;
                }

                int tmp = c - '0';

                if (tmp >= 0 && tmp <= 9)
                {
                    result *= 10;
                    result += tmp;
                }
                else if (option.HasFlag(ParseIntOption.IGNORE_INVALID_CHARACTERS))
                {
                    continue;
                }
                else
                {
                    result = 0;
                    return false;
                }

            }
            if (minus)
            {
                result *= -1;
            }
            return true;
        }
    }
}
