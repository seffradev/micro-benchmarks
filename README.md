# Micro-benchmarks in C++ using Google Benchmark

Clone the project, ensure you have `xmake` installed and just
run `xmake test -v` to run all benchmarks and tests.

The benchmarks are currently counted as tests to not run them
when running any default programs (though that would be
reasonable in this repo), and I couldn't figure out another
clever way using `xmake` to run them separately from the
tests. If you know how, please get in touch!
