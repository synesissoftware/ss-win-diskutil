# ss-win-diskutil - Changes <!-- omit in toc -->


## 0.2.6 - 11th October 2026

* Removed the **FastFormat**, **Pantheios**, **Pantheios.Extras.DiagUtil**, **Pantheios.Extras.Main**, **STLSoft**, **shwild**, **xTests**, and **b64** build dependencies;
* Removed the **test/scratch/list_drives** program, folding its `--verbose` diagnostics into **examples/list_volumes**, which now uses **Diagnosticism** (`diagnosticism_trace()`) and **woad** (stream-conditional colour) - for the examples only, never the library target;
* Removed the Visual Studio 2010 and 2015 solutions, projects, property sheets, and the **clcp** helper, along with the **implicit_link.h** header and its translation units, leaving CMake as the sole build system;
* Removed the unused `wininet` link and the **STLSoft** / **xTests** links from the CMake program macros, and the now-unused `define_automated_test_program()`;
* Changed CI to install only **Diagnosticism** and **woad**;


## 0.2.5-beta1 - 9th October 2026

* Added the full SisClr CMake helper corpus, including native Windows runners and separate unit, component, example, performance, and scratch test categories;
* Added the `run_all_automated_tests.*` aggregate runners for unit and component tests;
* Added the canonical `test/scratch/versions` composite version reporter;
* Named its CMake target `test.scratch.versions` so the scratch runner discovers `test.scratch.versions.exe`;
* Changed the current version to the computed `0.2.5-beta1` form in **include/ss-win-diskutil/version.h**;
* Updated CMake version extraction to accept the documented `VER_PATCH` macro;
* Quoted helper script directory resolution so project paths containing spaces are handled correctly;
* Applied the shared C / C++ editor, Git, and `.sis` boilerplate;
* Refreshed **.gitattributes**, **.gitignore**, **.vimrc**, and **.vscode/settings.json** for the shared project conventions;
* Added Windows MSVC and MinGW GitHub Actions CI with install-smoke coverage;


## 0.2.4 - 2nd February 2025

* Added CMake build support;
* Added GCC compatibility;


## 0.2.3 - 6th February 2024

* Applied minor improvements to project presentation;


## 0.2.2 - 5th August 2018

* Maintained the Windows disk-volume API and build support;


## 0.2.1.4 - 3rd August 2019

* Maintained the 0.2.1 release line;


## 0.2.1.3 - 3rd August 2019

* Maintained the 0.2.1 release line;


## 0.2.1.2 - 3rd August 2019

* Maintained the 0.2.1 release line;


## 0.2.1.1 - 3rd August 2019

* Maintained the 0.2.1 release line;


## 0.2.1 - 3rd August 2019

* Began tracked change history for the library;


<!-- ########################### end of file ########################### -->
