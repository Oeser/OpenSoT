#ifndef _WB_SOT_SOLVERS_QP_OASES_PROBLEM_H_
#define _WB_SOT_SOLVERS_QP_OASES_PROBLEM_H_

#include <Eigen/Dense>
#include <boost/shared_ptr.hpp>
#include <OpenSoT/Task.h>

#define DEFAULT_EPS_REGULARISATION 2E2

namespace qpOASES {
    class SQProblem;
    class Options;
    class Bounds;
    class Constraints;
}

namespace OpenSoT{
    namespace solvers{

    /**
     * @brief The QPOasesProblem class handle variables, options and execution of a
     * single qpOases problem. Is implemented using Eigen.
     * This represent the Back-End.
     */
    class QPOasesProblem {
    public:
        /**
         * @brief QPOasesProblem constructor with creation of a QP problem.
         * @param number_of_variables of the QP problem
         * @param number_of_constraints of the QP problem
         * @param hessian_type of the QP problem
         * @param eps_regularization set the Scaling factor of identity matrix used for Hessian regularisation.
         *             final_eps_regularisation = standard_eps_regularisation * eps_regularisation
         *        this parameter is particular important for the optimization!
         */
        QPOasesProblem(const int number_of_variables,
                       const int number_of_constraints,
                       OpenSoT::HessianType hessian_type = OpenSoT::HST_UNKNOWN,
                       const double eps_regularisation = DEFAULT_EPS_REGULARISATION); //2E2

        /**
          * @brief ~QPOasesProblem destructor
          */
        ~QPOasesProblem();

        /**
         * @brief setDefaultOptions to internal qpOases problem.
         * Default are set to:
         *  opt.setToMPC();
         *  opt.printLevel = qpOASES::PL_NONE;
         *  opt.enableRegularisation = qpOASES::BT_TRUE;
         *  opt.epsRegularisation *= _epsRegularisation;
         *  opt.numRegularisationSteps = 2;
         *  opt.numRefinementSteps = 1;
         *  opt.enableFlippingBounds = qpOASES::BT_TRUE;
         *
         */
        void setDefaultOptions();

        /**
         * @brief getProblem return the internal QP problem
         * @return reference to internal QP problem
         */
        const boost::shared_ptr<qpOASES::SQProblem>& getProblem(){return _problem;}

        /**
         * @brief getOptions return the options of the QP problem
         * @return options
         */
        qpOASES::Options getOptions();

        /**
         * @brief setOptions of the QP problem.
         * @param options
         */
        void setOptions(const qpOASES::Options& options);

        /**
         * @brief initProblem initialize the QP problem and get the solution, the dual solution,
         * bounds and constraints.
         * The QP problem has the following structure:
         *
         *      min = ||Hx - g||
         *  st.     lA <= Ax <= uA
         *           l <=  x <= u
         * @param H Task Matrix
         * @param g Task references
         * @param A Constraint Matrix
         * @param lA lower constraint Eigen::VectorXd
         * @param uA upper constraint Eigen::VectorXd
         * @param l lower bounds
         * @param u upper bounds
         * @return true if the problem can be solved
         */
        bool initProblem(const Eigen::MatrixXd& H, const Eigen::VectorXd& g,
                        const Eigen::MatrixXd& A,
                        const Eigen::VectorXd& lA, const Eigen::VectorXd& uA,
                        const Eigen::VectorXd& l, const Eigen::VectorXd& u);

        /**
         * This set of function update current problem copying input data. Use these
         * methods to update existing matrices of the QP problem.
         */

        /**
         * @brief updateTask update internal H and g:
         * _H = H
         * _g = g
         * for now is not possible to have different size of H and g wrt internal ones
         * @param H updated task matrix
         * @param g updated reference Eigen::VectorXd
         * @return true if task is correctly updated
         */
        bool updateTask(const Eigen::MatrixXd& H, const Eigen::VectorXd& g);

        /**
         * @brief updateConstraints update internal A, lA and uA
         * _A = A
         * _lA = lA
         * _uA = uA
         * A, lA and uA can change rows size to allow variable constraints
         * @param A update constraint matrix
         * @param lA update lower constraint Eigen::VectorXd
         * @param uA update upper constraint Eigen::VectorXd
         * @return true if constraints are correctly updated
         */
        bool updateConstraints(const Eigen::Ref<const Eigen::MatrixXd>& A, 
                               const Eigen::Ref<const Eigen::VectorXd> &lA, 
                               const Eigen::Ref<const Eigen::VectorXd> &uA);

        /**
         * @brief updateBounds update internal l and u
         * _l = l
         * _u = u
         * @param l update lower bounds
         * @param u update upper bounds
         * @return true if bounds are correctly updated
         */
        bool updateBounds(const Eigen::VectorXd& l, const Eigen::VectorXd& u);

        /**
         * @brief updateProblem update the whole problem see updateTask(), updateConstraints() and updateBounds()
         * @param H updated task matrix
         * @param g updated reference Eigen::VectorXd
         * @param A update constraint matrix
         * @param lA update lower constraint Eigen::VectorXd
         * @param uA update upper constraint Eigen::VectorXd
         * @param l update lower bounds
         * @param u update upper bounds
         * @return if the problem is correctly updated
         */
        bool updateProblem(const Eigen::MatrixXd& H, const Eigen::VectorXd& g,
                           const Eigen::MatrixXd& A,
                           const Eigen::VectorXd& lA, const Eigen::VectorXd& uA,
                           const Eigen::VectorXd& l, const Eigen::VectorXd& u);

        /**
         * @brief solve the QP problem
         * @return true if the QP problem is solved
         */
        bool solve();

        /**
         * @brief getSolution return the actual solution of the QP problem
         * @return solution
         */
        const Eigen::VectorXd& getSolution(){return _solution;}

        /**
         * @brief getHessianType return the hessian type f the problem
         * @return hessian type
         */
        OpenSoT::HessianType getHessianType();

        /**
         * @brief setHessianType of the problem
         * @param ht hessian type
         */
        void setHessianType(const OpenSoT::HessianType ht);

        /**
         * @brief getnWSR return maximum number of working set recalculations
         * @return maximum number of working set recalculations
         */
        int getnWSR(){return _nWSR;}

        /**
         * @brief setnWSR set maximum number of working set recalculations
         * @param nWSR Maximum number of working set recalculations
         */
        void setnWSR(const int nWSR){_nWSR = nWSR;}

        /**
         * @brief getActiveBounds return the active bounds of the solved QP problem
         * @return active bounds
         */
        const qpOASES::Bounds& getActiveBounds(){return *_bounds;}

        /**
         * @brief getActiveConstraints return the active constraints of the solved QP problem
         * @return active constraints
         */
        const qpOASES::Constraints& getActiveConstraints(){return *_constraints;}

        /**
         * Getters for internal matrices and Eigen::VectorXds
         */
        const Eigen::MatrixXd& getH(){return _H;}
        const Eigen::VectorXd& getg(){return _g;}
        const Eigen::MatrixXd& getA(){return _A;}
        const Eigen::VectorXd& getlA(){return _lA;}
        const Eigen::VectorXd& getuA(){return _uA;}
        const Eigen::VectorXd& getl(){return _l;}
        const Eigen::VectorXd& getu(){return _u;}

        /**
         * @brief printProblemInformation print some extra information about the problem
         * @param problem_number a number to identify the problem
         * @param problem_id a string to identify the problem
         * @param constraints_id a string to identify the constraints associated to the problem
         * @param bounds_id a string to identify the bounds associated to the problem
         */
        void printProblemInformation(const int problem_number, const std::string& problem_id,
                                     const std::string& constraints_id, const std::string& bounds_id);


        /**
         * @brief log Tasks, Constraints and Bounds matrices
         * @param logger a pointer to a MatLogger
         * @param i an index related to the particular index of the problem
         */
        void log(XBot::MatLogger::Ptr logger, int i)
        {
            logger->add("H_"+std::to_string(i), _H);
            logger->add("g_"+std::to_string(i), _g);
            if(_A.rows() > 0 && _A.cols() > 0)
                logger->add("A_"+std::to_string(i), _A);
            if(_lA.size() > 0)
                logger->add("lA_"+std::to_string(i), _lA);
            if(_uA.size() > 0)
                logger->add("uA_"+std::to_string(i), _uA);
            if(_l.size() > 0)
                logger->add("l_"+std::to_string(i), _l);
            if(_u.size() > 0)
                logger->add("u_"+std::to_string(i), _u);
            if(_solution.size() > 0)
                logger->add("solution_"+std::to_string(i), _solution);
        }

    protected:
        /**
         * @brief checkInfeasibility function that print informations when the problem is not feasible
         */
        void checkInfeasibility();

        /**
         * @brief checkINFTY if a bound/constraint is set to a value less than -INFTY then the bound/constraint is
         * set to -INFTY, if a bound/constraint is set to a value more than INFTY then the bound/constraint is
         * set to INFTY.
         */
        void checkINFTY();

        /**
         * @brief _problem is the internal SQProblem
         */
        boost::shared_ptr<qpOASES::SQProblem> _problem;

        /**
         * @brief _bounds are the active bounds of the SQProblem
         */
        boost::shared_ptr<qpOASES::Bounds> _bounds;

        /**
         * @brief _constraints are the active constraints of the SQProblem
         */
        boost::shared_ptr<qpOASES::Constraints> _constraints;

        /**
         * @brief _nWSR is the maximum number of working set recalculations
         */
        int _nWSR;

        /**
         * @brief _epsRegularisation is a factor that multiplies standard epsRegularisation of qpOases
         */
        double _epsRegularisation;

        /**
         * Define a cost function: ||Hx - g||
         */
        Eigen::MatrixXd _H;
        Eigen::VectorXd _g;

        /**
         * Define a set of constraints weighted with A: lA <= Ax <= uA
         */
        Eigen::MatrixXd _A;
        Eigen::VectorXd _lA;
        Eigen::VectorXd _uA;

        /**
         * Define a set of bounds on solution: l <= x <= u
         */
        Eigen::VectorXd _l;
        Eigen::VectorXd _u;

        /**
         * Solution and dual solution of the QP problem
         */
        Eigen::VectorXd _solution;
        Eigen::VectorXd _dual_solution;

        /**
         * @brief _opt solver options
         */
        boost::shared_ptr<qpOASES::Options> _opt;

        inline void pile(Eigen::MatrixXd& A, const Eigen::MatrixXd& B)
        {
            A.conservativeResize(A.rows()+B.rows(), A.cols());
            A.block(A.rows()-B.rows(),0,B.rows(),A.cols())<<B;
        }

        inline void pile(Eigen::VectorXd &a, const Eigen::VectorXd &b)
        {
            a.conservativeResize(a.rows()+b.rows());
            a.segment(a.rows()-b.rows(),b.rows())<<b;
        }
    };
    }
}
#endif
