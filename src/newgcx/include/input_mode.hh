#ifndef HOLODECK_X_NEW_CGX_INPUT_MODE_HH_
#define HOLODECK_X_NEW_CGX_INPUT_MODE_HH_



namespace holodeck_x
{
    enum class InputFormat
    {
        kUnknown,
        kAnsys,
        KAbaqus,
        kDuns2D,
        kDuns3D,
        kLsDyna,
        kOpenFoam,
        kIsaac2D,
        kIsaac3D,
        kNastran,
        kNetGen,
        kStep,
        kStepSplit,
        kStl,
        kVtk,
    };
}

#endif // HOLODECK_X_NEW_CGX_INPUT_MODE_HH_
