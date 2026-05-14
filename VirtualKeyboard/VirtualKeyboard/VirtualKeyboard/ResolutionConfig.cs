using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirtualKeyboard
{
    static class ResolutionConfig
    {
        public static readonly ReadOnlyDictionary<(double xRes, double yRes), (double alphaMin, double alphaMax, double numericMin, double numericMax)> ResolutionToSpacing = new(new Dictionary<(double xRes, double yRes), (double alphaMin, double alphaMax, double numericMin, double numericMax)>()
        {
            { (1920,1080), (0,15,0,15)},
            //{(1680,1050),(0,15,0,15) },
            //{(1440,900),(0, 15, 0, 15) },
            //{(1400,1050),(0, 15, 0, 15) },
            //{(1366,768),(0, 15, 0, 15) },
            //{(1369,768),(0, 15, 0, 15) },
            //{(1280,1024),(0, 15, 0, 15) },
            //{(1280,960),(0, 15, 0, 15) },
            // {(1280,800),(0, 15, 0, 15) },
            //{(1280,768),(0, 15, 0, 15) },
            //{(1280,720),(0, 15, 0, 15) },
            //{(1280,600),(0, 15, 0, 15) },
            // {(1152,864),(0, 15, 0, 15) },

            {(1024,768),(0, 10, 0, 15) },

            {(800,600),(0, 7.5, 0, 15) },

        });

        public static readonly ReadOnlyDictionary<(double xRes, double yRes), (double alphaMin, double alphaMax, double numericMin, double numericMax)> ResolutionToFontSize = new(new Dictionary<(double xRes, double yRes), (double alphaMin, double alphaMax, double numericMin, double numericMax)>()
        {
            { (1920,1080), (0, 60, 0, 100) },
            //{(1680,1050),(5, 60, 5, 60) },
            //{(1440,900),(5, 60, 5, 60) },
            //{(1400,1050),(5, 60, 5, 60) },
            //{(1366,768),(5, 60, 5, 60) },
            //{(1369,768),(5, 60, 5, 60) },
            //{(1280,1024),(5, 60, 5, 60) },
            //{(1280,960),(5, 60, 5, 60) },
            // {(1280,800),(5, 60, 5, 60) },
            //{(1280,768),(5, 60, 5, 60) },
            //{(1280,720),(5, 60, 5, 60) },
            //{(1280,600),(5, 60, 5, 60)},
            // {(1152,864),(5, 60, 5, 60) },

            {(1024,768),(0, 40, 0, 100) },

            {(800,600),(0, 30, 0, 60) },

        });


        public static readonly ReadOnlyDictionary<(double xRes, double yRes), (double alphaRatio, double numericRatio)> ResolutionToKeyboardRatio = new(new Dictionary<(double xRes, double yRes), (double alphaRatio, double numericRatio)>()
        {
            { (1920,1080), (0.35,1.2) },
            //{(1680,1050),(5, 60, 5, 60) },
            //{(1440,900),(5, 60, 5, 60) },
            //{(1400,1050),(5, 60, 5, 60) },
            //{(1366,768),(5, 60, 5, 60) },
            //{(1369,768),(5, 60, 5, 60) },
            //{(1280,1024),(5, 60, 5, 60) },
            //{(1280,960),(5, 60, 5, 60) },
            // {(1280,800),(5, 60, 5, 60) },
            //{(1280,768),(5, 60, 5, 60) },
            //{(1280,720),(5, 60, 5, 60) },
            //{(1280,600),(5, 60, 5, 60)},
            // {(1152,864),(5, 60, 5, 60) },

            {(1024,768),(0.35,1.2) },

            {(800,600),(0.35,1.2) },

        });

    }
}
