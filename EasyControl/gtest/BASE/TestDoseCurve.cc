#include "pch.h"
#include <cassert>
#include "BASE/math/public/DoseCurve.h"

using namespace base;


#define CURVESIZE 5U

namespace
{
    float32_t fy(float32_t x, float32_t x0, float32_t x1, float32_t y0, float32_t y1)
    {
        float64_t m = (y1 - y0) / (x1 - x0);
        float64_t b = y1 - m * x1;
        return _F32(m * x + b);
    }

    TEST(DoseCurve, GetDriveCommand)
    {
        base::math::CDoseCurve<CURVESIZE> aCurve;

        std::array<float32_t, CURVESIZE> dc { 0.1F, 0.2F, 0.3F, 0.4F, 0.5F };
        std::array<float32_t, CURVESIZE> qm { 100.0F, 120.0F, 130.0F, 120.0F, 100.0F };
        std::array<float32_t, CURVESIZE> mv{ 10.0F, 24.0F, 39.0F, 48.0F, 50.0F };
        std::array<float32_t, CURVESIZE> av;


        aCurve.Init();

        for (int32_t i = 0; i < CURVESIZE; i++)
        {
            aCurve.Push({ dc[i], qm[i] });
            av[i] = dc[i] * qm[i];
            EXPECT_TRUE(av[i] == mv[i]);
        }
        aCurve.SortDriveCommand();
        for (int32_t i = 0; i < CURVESIZE; i++)
        {
            auto value = aCurve.GetDriveCommand(av[i]);
            EXPECT_TRUE(fabs(dc[i] - value) < 1e-09);
        }
        int32_t index = 0;
        float32_t c = 10.0F;
        auto value = aCurve.GetDriveCommand(c);
        float32_t f = dc[index];
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        c = 5.0F;
        value = aCurve.GetDriveCommand(c);
        f = fy(c, 0, av[index], 0, dc[index]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        c = 15.0F;
        value = aCurve.GetDriveCommand(c);
        f = fy(c, av[index], av[index+1], dc[index], dc[index+1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        index = 1;

        c = 25.0F;
        value = aCurve.GetDriveCommand(c);
        f = fy(c, av[index], av[index+1], dc[index], dc[index+1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        c = 38.0F;
        value = aCurve.GetDriveCommand(c);
        f = fy(c, av[index], av[index + 1], dc[index], dc[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        index = 2;

        c = 41.0F;
        value = aCurve.GetDriveCommand(c);
        f = fy(c, av[index], av[index + 1], dc[index], dc[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        c = 47.0F;
        value = aCurve.GetDriveCommand(c);
        f = fy(c, av[index], av[index + 1], dc[index], dc[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        index = 3;

        c = 49.0F;
        value = aCurve.GetDriveCommand(c);
        f = fy(c, av[index], av[index + 1], dc[index], dc[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        index = 4;

        c = 51.0F;
        value = aCurve.GetDriveCommand(c);
        f = fy(c, av[index-1], av[index], dc[index-1], dc[index]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

    }


    TEST(DoseCurve, GetPerformance)
    {
        base::math::CDoseCurve<CURVESIZE> aCurve;

        std::array<float32_t, CURVESIZE> dc{ 0.1F, 0.2F, 0.3F, 0.4F, 0.5F };
        std::array<float32_t, CURVESIZE> qm{ 100.0F, 120.0F, 130.0F, 120.0F, 100.0F };
        std::array<float32_t, CURVESIZE> mv{ 10.0F, 24.0F, 39.0F, 48.0F, 50.0F };
        std::array<float32_t, CURVESIZE> av;


        aCurve.Init();

        for (int32_t i = 0; i < CURVESIZE; i++)
        {
            aCurve.Push({ dc[i], qm[i] });
            av[i] = dc[i] * qm[i];
            EXPECT_TRUE(av[i] == mv[i]);
        }
        aCurve.SortDriveCommand();
        for (int32_t i = 0; i < CURVESIZE; i++)
        {
            auto value = aCurve.GetDosePerformance(av[i]);
            EXPECT_TRUE(fabs(qm[i] - value) < 1e-09);
        }
        int32_t index = 0;
        float32_t c = 10.0F;
        auto value = aCurve.GetDosePerformance(c);
        float32_t f = qm[index];
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        c = 5.0F;
        value = aCurve.GetDosePerformance(c);
        f = qm[index];
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        c = 15.0F;
        value = aCurve.GetDosePerformance(c);
        f = fy(c, av[index], av[index + 1], qm[index], qm[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        index = 1;

        c = 25.0F;
        value = aCurve.GetDosePerformance(c);
        f = fy(c, av[index], av[index + 1], qm[index], qm[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        c = 38.0F;
        value = aCurve.GetDosePerformance(c);
        f = fy(c, av[index], av[index + 1], qm[index], qm[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        index = 2;

        c = 41.0F;
        value = aCurve.GetDosePerformance(c);
        f = fy(c, av[index], av[index + 1], qm[index], qm[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        c = 47.0F;
        value = aCurve.GetDosePerformance(c);
        f = fy(c, av[index], av[index + 1], qm[index], qm[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        index = 3;

        c = 49.0F;
        value = aCurve.GetDosePerformance(c);
        f = fy(c, av[index], av[index + 1], qm[index], qm[index + 1]);
        EXPECT_TRUE(fabs(value - f) < 1e-06);

        index = 4;

        c = 51.0F;
        value = aCurve.GetDosePerformance(c);
        f = qm[index];
        EXPECT_TRUE(fabs(value - f) < 1e-06);

    }

}
