
#include <pantheios/pan.hpp>

#include <ss-win-diskutil.h>

#include <fastformat/ff.hpp>
#include <fastformat/sinks/iostream.hpp>

#include <pantheios/extras/diagutil.hpp>
#include <pantheios/extras/main.hpp>

#include <stlsoft/conversion/truncation_cast.hpp>
#include <stlsoft/conversion/w2m.hpp>
#include <stlsoft/smartptr/scoped_handle.hpp>

#include <stdlib.h>

#include <crtdbg.h>

inline unsigned long st2ul(size_t n)
{
    return stlsoft::truncation_cast<unsigned long>(n);
}

static
int
main_(int argc, char* argv[]);

static
int
main_leak_traced(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    return pantheios::extras::main::invoke(argc, argv, main_leak_traced);
}

static
int
main_leak_traced(int argc, char* argv[])
{
    return pantheios::extras::diagutil::main_leak_trace::invoke(argc, argv, main_);
}

static
int
main_(int /*argc*/, char* /*argv*/[])
{
    using namespace ::SynesisSoftware::Windows::DiskUtil;

    VolumeDescriptions_t volumes;

    int r = LoadVolumes(0, &volumes);

    if (0 != r)
    {
        stlsoft::scoped_handle<VolumeDescriptions_t> scoper(volumes, ReleaseVolumes);

        ff::fmtln(std::cout, "Volumes [{0}]:", volumes->numVolumes);

        for (size_t i = 0; volumes->numVolumes != i; ++i)
        {
            auto const* vd = &volumes->volumes[i];

            ff::fmtln(std::cout, "\t{0} - '{1}' : {2} of {3}", stlsoft::w2m(vd->id), stlsoft::w2m(vd->friendlyName), vd->systemFreeBytes, vd->capacityBytes);
        }
    }

    return EXIT_SUCCESS;
}

