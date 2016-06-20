#r "CommonMark.dll"

using System.Text.RegularExpressions;


    public static class JpExt
    {
        private static class RE
        {
            private const string A = "["
                + @"\p{IsCJKUnifiedIdeographs}"
                + @"\p{IsCJKCompatibilityIdeographs}"
                + @"\p{IsCJKUnifiedIdeographsExtensionA}"
                + @"\p{IsHiragana}"
                + @"\p{IsKatakana}"
                + @"\p{P}"
                + @"\u31F0-\u31FF\u3099-\u309C\uFF65-\uFF9F"
                + "]";

            private const string B = @"[\uD840-\uD869][\uDC00-\uDFFF]";
            private const string C = @"\uD869[\uDC00-\uDEDF]";

            private const string JP = A + "|" + B + "|" + C;

            private const string JP1 = "(?<JP1>" + JP + ")";
            private const string JP2 = "(?<JP2>" + JP + ")";
            private const string CRLF = @"\r\n?";

            public static Regex JpLfJp = new Regex(JP1 + CRLF + JP2, RegexOptions.Compiled);
        }

        public static string RemoveCRLF(string src)
        {
            var dst = RE.JpLfJp.Replace(src, (m) =>
             {
                 var JP1 = m.Groups["JP1"].Value;
                 var JP2 = m.Groups["JP2"].Value;
                 return JP1 + JP2;
             });
            return dst;
        }
    }




[Export(typeof(ILightweightMarkupEngine))]
public sealed class CustomCommonMarkEngine : ILightweightMarkupEngine
{
    public string Convert(string source)
    {
        var t1 = CommonMark.CommonMarkConverter.Convert(source);
        var t2 = JpExt.RemoveCRLF(t1);
        return t2;
    }
}