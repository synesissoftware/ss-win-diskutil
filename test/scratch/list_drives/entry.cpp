
#include <pantheios/pantheios.hpp>

#include <ss-win-diskutil.h>

#include <fastformat/fastformat.h>
#include <fastformat/sinks/iostream.hpp>
#include <fastformat/ff.hpp>

#include <platformstl/filesystem/path_functions.h>
#include <stlsoft/conversion/truncation_cast.hpp>
#include <stlsoft/conversion/w2m.hpp>
#include <stlsoft/smartptr/scoped_handle.hpp>

#if defined(HAS_Pantheios_Extras_DiagUtil) ||\
    defined(HAS_DIAGUTIL)

# include <pantheios/extras/diagutil.hpp>
#endif
#include <pantheios/extras/main.hpp>
#include <pantheios/inserters/integer.hpp>
#include <pantheios/frontends/fe.simple.h>

#include <stdio.h>
#include <stdlib.h>


static
int
main_(int argc, char* argv[]);

#if defined(HAS_Pantheios_Extras_DiagUtil) ||\
    defined(HAS_DIAGUTIL)

static
int
main_leak_traced(int argc, char* argv[]);
#endif

int main(int argc, char* argv[])
{
#if defined(HAS_Pantheios_Extras_DiagUtil) ||\
    defined(HAS_DIAGUTIL)

    return pantheios::extras::main::invoke(argc, argv, main_leak_traced);
}

static
int
main_leak_traced(int argc, char* argv[])
{
    return pantheios::extras::diagutil::main_leak_trace::invoke(argc, argv, main_);
#else

    return pantheios::extras::main::invoke(argc, argv, main_);
#endif
}

static
int
main_(int argc, char* argv[])
{
    bool log_all = false;

    for (int i = 1; i != argc; ++i)
    {
        if (0 == ::strcmp("--help", argv[i]))
        {
            auto const program_name = platformstl::get_executable_name_from_path(argv[0]);

            ff::fmtln(std::cout, "USAGE: {0} {{ | --help | --log-all }\n", program_name);

            return EXIT_SUCCESS;
        }

        if (0 == ::strcmp("--log-all", argv[i]))
        {
            log_all = true;
        }
    }

    if (log_all)
    {
        pantheios_fe_simple_setSeverityCeiling(PANTHEIOS_SEV_DEBUG);
    }
    else
    {
        pantheios_fe_simple_setSeverityCeiling(PANTHEIOS_SEV_ERROR);
    }

    using namespace ::SynesisSoftware::Windows::DiskUtil;

    VolumeDescriptions_t volumes;

    pantheios::log_NOTICE("loading volumes ...");

    int r = LoadVolumes(0, &volumes);

    if (0 != r)
    {
        stlsoft::scoped_handle<VolumeDescriptions_t> scoper(volumes, ReleaseVolumes);

        pantheios::log_DEBUG("obtained information for ", pantheios::integer(volumes->numVolumes), " volume(s) ...");

        ff::fmtln(std::cout, "Volumes [{0}]:", volumes->numVolumes);

        for (size_t i = 0; volumes->numVolumes != i; ++i)
        {
            auto const* const vd = &volumes->volumes[i];

            ff::fmtln(std::cout, "\t{0} - '{1}' : {2} of {3}", stlsoft::w2m(vd->id), stlsoft::w2m(vd->friendlyName), vd->systemFreeBytes, vd->capacityBytes);
        }
    }

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

