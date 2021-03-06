#ifndef KGeoBag_KGDiscreteRotationalMeshElement_hh_
#define KGeoBag_KGDiscreteRotationalMeshElement_hh_

#include "KTransformation.hh"

#include <vector>
using std::vector;

namespace KGeoBag
{

    class KGMeshElement;

    class KGDiscreteRotationalMeshElement
    {
        public:
            KGDiscreteRotationalMeshElement();
            virtual ~KGDiscreteRotationalMeshElement();

            virtual double Area() const = 0;
            virtual double Aspect() const = 0;

            virtual KGMeshElement& Element() = 0;

            virtual void Transform( const KTransformation& transform ) = 0;
    };

    typedef vector< KGDiscreteRotationalMeshElement* > KGDiscreteRotationalMeshElementVector;
    typedef vector< KGDiscreteRotationalMeshElement* >::iterator KGDiscreteRotationalMeshElementIt;
    typedef vector< KGDiscreteRotationalMeshElement* >::const_iterator KGDiscreteRotationalMeshElementCIt;

  template <class XMeshElement>
  class KGDiscreteRotationalMeshElementType :
    public KGDiscreteRotationalMeshElement
  {
  public:
    KGDiscreteRotationalMeshElementType(const XMeshElement& element) :
      KGDiscreteRotationalMeshElement(),
      fMeshElement(element),
      fNElements(1) {}
    virtual ~KGDiscreteRotationalMeshElementType() {}

    XMeshElement& Element() { return fMeshElement; }

    double Area() const { return fMeshElement.Area(); }
    virtual double Aspect() const { return fMeshElement.Aspect(); }

    virtual void Transform( const KTransformation& transform ) { fMeshElement.Transform(transform); }

    void NumberOfElements(unsigned int i) { fNElements = i; }
    unsigned int NumberOfElements() const { return fNElements; }

  private:
    XMeshElement fMeshElement;
    unsigned int fNElements;
  };

}

#include "KGMeshRectangle.hh"
#include "KGMeshTriangle.hh"
#include "KGMeshWire.hh"

namespace KGeoBag
{
  typedef KGDiscreteRotationalMeshElementType<KGMeshRectangle> KGDiscreteRotationalMeshRectangle;
  typedef KGDiscreteRotationalMeshElementType<KGMeshTriangle> KGDiscreteRotationalMeshTriangle;
  typedef KGDiscreteRotationalMeshElementType<KGMeshWire> KGDiscreteRotationalMeshWire;
}

#endif
