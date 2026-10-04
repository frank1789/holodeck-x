#ifndef HOLODECK_X_ELEMENT_TYPE_HH
#define HOLODECK_X_ELEMENT_TYPE_HH

#include <cstdint>

namespace holodeck_x {

    enum class ElementType : std::uint8_t
    {
        kHexa8,
        kPe6,
        KTet4,
        kHexa20,
        kPe15,
        kTet10,
        kTri3,
        KTri6,
        KQuad4,
        KQuad8,
        kBeam2,
        KBeam3,
    };
} // namespace holodeck_x

#endif // HOLODECK_X_ELEMENT_TYPE_HH
