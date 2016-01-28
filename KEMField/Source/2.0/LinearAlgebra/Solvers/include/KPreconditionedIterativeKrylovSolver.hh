#ifndef KPreconditionedIterativeKrylovSolver_HH__
#define KPreconditionedIterativeKrylovSolver_HH__

#include "KSquareMatrix.hh"
#include "KVector.hh"

#include "KIterativeSolver.hh"
#include "KPreconditioner.hh"
#include "KIterativeKrylovRestartCondition.hh"

namespace KEMField
{

/*
*
*@file KPreconditionedIterativeKrylovSolver.hh
*@class KPreconditionedIterativeKrylovSolver
*@brief controller class for right-preconditoned krylov solvers
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Fri Jan 31 15:27:04 EST 2014 J. Barrett (barrettj@mit.edu) First Version
*
*/

template <typename ValueType, template <typename> class ParallelTrait>
class KPreconditionedIterativeKrylovSolver: public KIterativeSolver<ValueType>
{
    public:
        typedef KSquareMatrix<ValueType> Matrix;
        typedef KVector<ValueType> Vector;
        typedef KPreconditioner<ValueType> Preconditioner;

        KPreconditionedIterativeKrylovSolver();
        virtual ~KPreconditionedIterativeKrylovSolver();

        void Solve(const Matrix& A, Preconditioner& P, Vector& x, const Vector& b);

        //set tolerancing (default is absolute tolerance on l2 residual norm)
        virtual void SetTolerance(double d) { SetRelativeTolerance(d); };
        virtual void SetAbsoluteTolerance(double d){ this->fTolerance = d; fUseRelativeTolerance = false;};
        virtual void SetRelativeTolerance(double d){ this->fTolerance = d; fUseRelativeTolerance = true;};

        virtual double ResidualNorm() const
        {
            if(fUseRelativeTolerance)
            {
                return (this->fResidualNorm)/fInitialResidual;
            }
            else
            {
                return this->fResidualNorm;
            }
        }

        void SetMaximumIterations(unsigned int i){fMaxIterations = i;}
        void SetRestartCondition(KIterativeKrylovRestartCondition* restart)
        {
            if(!fExternalRestartCondition){delete fRestartCondition; fExternalRestartCondition = true;};
            fRestartCondition = restart;
        };

        ParallelTrait<ValueType>* GetTrait() {return fTrait;};

    private:
        unsigned int Dimension() const { return (fTrait ? fTrait->Dimension() : 0); }
        void SetResidualVector(const Vector& v) { if (fTrait) fTrait->SetResidualVector(v); }
        void GetResidualVector(Vector& v) { if (fTrait) fTrait->GetResidualVector(v); }

        virtual bool HasConverged()
        {
            if(fUseRelativeTolerance)
            {
                double relative_residual_norm = (this->fResidualNorm)/(fInitialResidual);
                if( relative_residual_norm > this->fTolerance ){return false;}
                else{return true;}
            }
            else
            {
                if( this->fResidualNorm > this->fTolerance ){ return false; }
                else{ return true; }
            }
        }

        double InnerProduct(const Vector& a, const Vector& b)
        {
            double result = 0.;

            unsigned int dim = a.Dimension();
            for(unsigned int i=0; i<dim; i++)
            {
                result += a(i)*b(i);
            }

            return result;
        }

        unsigned int fMaxIterations;
        KIterativeKrylovRestartCondition* fRestartCondition;
        ParallelTrait<ValueType>* fTrait;
        bool fExternalRestartCondition;
        bool fUseRelativeTolerance;
        double fInitialResidual;
  };

    template <typename ValueType,template <typename> class ParallelTrait>
    KPreconditionedIterativeKrylovSolver<ValueType,ParallelTrait>::KPreconditionedIterativeKrylovSolver():
        fMaxIterations(UINT_MAX),
        fRestartCondition(NULL),
        fTrait(NULL)
        {
            //create a default restart condition
            fRestartCondition = new KIterativeKrylovRestartCondition();
            fExternalRestartCondition = false;
            fUseRelativeTolerance = false;
            fInitialResidual = 1.0;
        };

    template <typename ValueType,template <typename> class ParallelTrait>
    KPreconditionedIterativeKrylovSolver<ValueType,ParallelTrait>::~KPreconditionedIterativeKrylovSolver()
    {
        if(!fExternalRestartCondition)
        {
            delete fRestartCondition;
        }
    };

    template <typename ValueType,template <typename> class ParallelTrait>
    void KPreconditionedIterativeKrylovSolver<ValueType,ParallelTrait>::Solve(const Matrix& A, Preconditioner& P, Vector& x, const Vector& b)
    {
        ParallelTrait<ValueType> trait(A,P,x,b);
        fTrait = &trait;

        this->InitializeVisitors();
        this->fIteration = 0;

        trait.Initialize();

        //needed if we are using relative tolerance as convergence condition
        fInitialResidual = std::sqrt( InnerProduct(b,b) );
        bool solutionUpdated = false;

        do
        {
            solutionUpdated = false;
            trait.AugmentKrylovSubspace();
            trait.GetResidualNorm(this->fResidualNorm);
            fRestartCondition->UpdateProgress(this->fResidualNorm);
            this->fIteration++;

            if( fRestartCondition->PerformRestart() )
            {
                trait.UpdateSolution();
                trait.ResetAndInitialize(); //clears krylov subspace vectors, restarts from current solution
            }
            else if( HasConverged() || (this->fIteration >= this->fMaxIterations) )
            {
                trait.UpdateSolution();
                solutionUpdated = true;
                break;
            }
            else if( this->Terminate() )
            {
                trait.UpdateSolution();
                solutionUpdated = true;
                break;
            }

            this->AcceptVisitors();
        }
        while( !( HasConverged() ) && (this->fIteration < this->fMaxIterations) && !(this->Terminate()) );

        if(!solutionUpdated){trait.UpdateSolution();};

        trait.Finalize();
        this->FinalizeVisitors();

        fTrait = NULL;
    }

}//end of KEMField namespace

#endif /* KPreconditionedIterativeKrylovSolver_H__ */
