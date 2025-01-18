/**
 * @file correlator.cpp
 * @author Tianchen (i@tinikov.com)
 * @brief
 * @version 0.1
 * @date 2025-01-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "bridge.h"
using Bridge::vout;

namespace {
const std::string test_name = "solve_correlator";
const std::string parameter_file_default = "corr_12x24.yaml";
}  // namespace

//====================================================================
void usage(const char *program_name) { vout.general("\n\nusage: %s parameter_file\n\n", program_name); }

//====================================================================
int hadron_2ptFunction(const Parameters &params_all) {
  // ####  parameter setup  ####
  const int Nc = CommonParameters::Nc();
  const int Nd = CommonParameters::Nd();
  const int Ndim = CommonParameters::Ndim();
  const int Nvol = CommonParameters::Nvol();

  const Parameters params_spectrum = params_all.lookup("Spectrum");
  const Parameters params_fopr = params_all.lookup("Fopr");
  const Parameters params_solver = params_all.lookup("Solver");
  const Parameters params_source = params_all.lookup("Source");

  const string str_gconf_read = params_spectrum.get_string("gauge_config_type_input");
  const string readfile = params_spectrum.get_string("config_filename_input");
  const string str_rand_type = params_spectrum.get_string("random_number_type");
  const unsigned long seed = params_spectrum.get_unsigned_long("seed_for_random_number");
  const string str_vlevel = params_spectrum.get_string("verbose_level");

  const string str_fopr_type = params_fopr.get_string("fermion_type");
  const string str_gmset_type = params_fopr.get_string("gamma_matrix_type");
  const string str_solver_type = params_solver.get_string("solver_type");
  const string str_source_type = params_source.get_string("source_type");

  const Bridge::VerboseLevel vl = vout.set_verbose_level(str_vlevel);

  //- print input parameters
  vout.general(vl, "  gconf_read   = %s\n", str_gconf_read.c_str());
  vout.general(vl, "  readfile     = %s\n", readfile.c_str());
  vout.general(vl, "  rand_type    = %s\n", str_rand_type.c_str());
  vout.general(vl, "  seed         = %lu\n", seed);
  vout.general(vl, "  vlevel       = %s\n", str_vlevel.c_str());
  vout.general(vl, "  gmset_type   = %s\n", str_gmset_type.c_str());
  vout.general(vl, "  solver_type  = %s\n", str_solver_type.c_str());
  vout.general(vl, "  source_type  = %s\n", str_source_type.c_str());

  RandomNumberManager::initialize(str_rand_type, seed);

  // ####  Set up a gauge configuration  ####
  Field_G U(Nvol, Ndim);
  GaugeConfig(str_gconf_read).read(U, readfile);

  // ####  object setup  #####
  unique_ptr<GammaMatrixSet> gmset(GammaMatrixSet::New(str_gmset_type));

  unique_ptr<Fopr> fopr(Fopr::New(str_fopr_type, params_fopr));
  fopr->set_config(&U);

  unique_ptr<Solver> solver(Solver::New(str_solver_type, fopr.get(), params_solver));

  unique_ptr<Fprop> fprop_lex(new Fprop_Standard_lex(solver.get()));

  unique_ptr<Source> source(Source::New(str_source_type, params_source));

  Corr2pt_4spinor corr(gmset.get(), params_all.lookup("Corr2pt_4spinor"));

  Timer timer(test_name);

  // ####  Execution main part  ####
  timer.start();

  std::vector<Field_F> sq(Nc * Nd);
  for (int i_cd = 0; i_cd < Nc * Nd; ++i_cd) {
    sq[i_cd].set(0.0);
  }

  vout.general(vl, "\n");
  vout.general(vl, "Solving quark propagator:\n");

  for (int ispin = 0; ispin < Nd; ++ispin) {
    for (int icolor = 0; icolor < Nc; ++icolor) {
      int i_cd = icolor + Nc * ispin;

      Field_F b;  // b.set(0.0);
      source->set(b, i_cd);

      int Nconv;
      double diff;
      fprop_lex->invert_D(sq[i_cd], b, Nconv, diff);

      Field_F y(b);
      fopr->set_mode("D");
      fopr->mult(y, sq[i_cd]);  // y  = fopr->mult(sq[i_cd]);
      axpy(y, -1.0, b);         // y -= b;
      double diff2 = y.norm2() / b.norm2();

      vout.general(vl, "  color spin   Nconv      diff           diff2\n");
      vout.general(vl, "   %2d   %2d   %6d   %12.4e   %12.4e\n\n", icolor, ispin, Nconv, diff, diff2);
    }
  }

  //- meson correlators
  vout.general(vl, "\n");
  vout.general(vl, "2-point correlator:\n");

  const double result = corr.meson_all(sq, sq);

  timer.report();

  RandomNumberManager::finalize();

  return EXIT_SUCCESS;
}

//====================================================================
int main(int argc, char *argv[]) {
  bridge_initialize(&argc, &argv);

  if (argc < 2) {
    usage(argv[0]);

    bridge_finalize();
    return EXIT_FAILURE;
  }

  std::string parameter_file = std::string(argv[1]);

  Parameters params = ParameterManager::read(parameter_file);
  bridge_setup(params.lookup("Main"));

  Timer timer("Main");
  timer.start();

  hadron_2ptFunction(params);

  timer.stop();
  timer.report();

  bridge_finalize();

  return EXIT_SUCCESS;
}
