/**
 * ============================================================
 *  sca_ls.cpp — SCA + Solis-Wets (algoritmo memético)
 * ============================================================
 *  Hibridación: SCA como explorador global + Solis-Wets como
 *  intensificador local activado por estancamiento.
 *
 *  Referencia SCA:
 *    S. Mirjalili, Knowledge-Based Systems, 2016.
 *  Referencia Solis-Wets:
 *    F.J. Solis y R.J.B. Wets, Math. of Op. Research, 1981.
 *
 *  Autor:  Jesús Muñoz Velasco
 *  Curso:  Metaheurísticas 2025-2026
 * ============================================================
 */

#include "cec17_common.h"

// ─────────────────────────────────────────────────────────────
// Parámetros específicos del SCA-LS
// ─────────────────────────────────────────────────────────────
static constexpr double A_PARAM         = 2.0;   // Amplitud inicial r1
static constexpr int    STAGNATION_GENS = 10;    // Generaciones sin mejora → LS
static constexpr double LS_BUDGET_RATIO = 0.10;  // Fracción del presupuesto por llamada LS

// ─────────────────────────────────────────────────────────────
// Clase SCA_LS
// ─────────────────────────────────────────────────────────────
class SCA_LS {
public:
    SCA_LS(int dim, long max_evals, unsigned int seed)
        : dim_(dim), max_evals_(max_evals),
          ls_budget_(static_cast<long>(max_evals * LS_BUDGET_RATIO)),
          gen_(seed),
          dis_pos_(LOWER_BOUND, UPPER_BOUND),
          dis_r2_(0.0, 2.0 * M_PI),
          dis_r3_(0.0, 2.0),
          dis_r4_(0.0, 1.0)
    {}

    double run() {
        Population pop(POP_SIZE, dim_, dis_pos_, gen_);
        int stagnation = 0;

        while (pop.evals < max_evals_) {
            double prev_best = pop.best_fit;
            double progress  = static_cast<double>(pop.evals) /
                               static_cast<double>(max_evals_);
            double r1 = A_PARAM * (1.0 - progress);

            // ── Fase SCA ────────────────────────────────────────────
            for (int i = 0; i < POP_SIZE; ++i) {
                if (pop.evals >= max_evals_) break;

                for (int j = 0; j < dim_; ++j) {
                    double r2   = dis_r2_(gen_);
                    double r3   = dis_r3_(gen_);
                    double r4   = dis_r4_(gen_);
                    double dist = std::abs(r3 * pop.best_pos[j] - pop.pos[i][j]);

                    if (r4 < 0.5) pop.pos[i][j] += r1 * std::sin(r2) * dist;
                    else          pop.pos[i][j] += r1 * std::cos(r2) * dist;

                    pop.pos[i][j] = clamp(pop.pos[i][j]);
                }

                pop.evaluate_and_update(i);
            }

            // ── Detección de estancamiento ───────────────────────────
            if (pop.best_fit < prev_best) stagnation = 0;
            else                          ++stagnation;

            // ── Fase LS (Solis-Wets) ─────────────────────────────────
            if (stagnation >= STAGNATION_GENS && pop.evals < max_evals_) {
                long ls_budget = std::min(ls_budget_, max_evals_ - pop.evals);
                long ls_evals  = 0;

                std::vector<double> ls_sol = pop.best_pos;
                double              ls_fit = pop.best_fit;

                solis_wets(ls_sol, ls_fit, ls_budget, ls_evals, gen_);
                pop.evals += ls_evals;
                pop.inject_best(ls_sol, ls_fit);

                stagnation = 0;
            }
        }

        return cec17_error(pop.best_fit);
    }

private:
    int  dim_;
    long max_evals_;
    long ls_budget_;

    std::mt19937                           gen_;
    std::uniform_real_distribution<double> dis_pos_;
    std::uniform_real_distribution<double> dis_r2_;
    std::uniform_real_distribution<double> dis_r3_;
    std::uniform_real_distribution<double> dis_r4_;
};

// ─────────────────────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────────────────────
int main(int argc, char *argv[]) {
    int dim       = 10;
    int seed_base = 42;
    if (argc >= 2) dim       = std::stoi(argv[1]);
    if (argc >= 3) seed_base = std::stoi(argv[2]);

    if (!valid_dim(dim)) {
        std::cerr << "[Error] Dimension invalida: " << dim
                  << ". Validos: 2, 5, 10, 30, 50, 100.\n";
        return 1;
    }

    std::cout << "========================================\n"
              << "  SCA-LS (SCA + Solis-Wets)\n"
              << "  Dimension   : " << dim << "\n"
              << "  Max. evals  : " << 10000L * dim << "\n"
              << "  Runs/func   : " << T_RUNS << "\n"
              << "  Pop. size   : " << POP_SIZE << "\n"
              << "  Stagnation  : " << STAGNATION_GENS << " gens\n"
              << "  LS budget % : " << (LS_BUDGET_RATIO * 100.0) << "%\n"
              << "========================================\n\n";

    run_algorithm("sca_ls", dim, seed_base,
        [](int d, long e, unsigned s){ return SCA_LS(d, e, s); });
}
