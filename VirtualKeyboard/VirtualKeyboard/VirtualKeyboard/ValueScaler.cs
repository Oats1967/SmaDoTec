using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirtualKeyboard
{
    internal static class ValueScaler
    {
        public static double MapLinear(double value, double fromSource, double toSource, double fromTarget, double toTarget)
        {
            // Ensure the value is within the source range
            value = Math.Max(fromSource, Math.Min(toSource, value));

            // Perform the mapping
            double mappedValue = (value - fromSource) / (toSource - fromSource) * (toTarget - fromTarget) + fromTarget;

            return mappedValue;
        }

        public static double MapLogarithmic(double value, double fromSource, double toSource, double fromTarget, double toTarget)
        {
            // Ensure the value is within the source range
            value = Math.Max(fromSource, Math.Min(toSource, value));

            // Perform the logarithmic mapping
            double mappedValue = Math.Pow((value - fromSource) / (toSource - fromSource), 2) * (toTarget - fromTarget) + fromTarget;

            return mappedValue;
        }



    }
}
