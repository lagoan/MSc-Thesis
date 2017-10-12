#ifndef ALPHASHAPEHDSPOLYHEDRONIFIKATOR_H_
#define ALPHASHAPEHDSPOLYHEDRONIFIKATOR_H_
/* Benjamin SCHWARZ (not that proud though) 
 *   7 april 2009 - Strasbourg, after OTAN party
 */
#include "AlphashapeHDSator.h"

template <class Alphashape, class Polyhedron> 
class AlphashapeHDSPolyhedronifikator
{
private :
	template <class polyHDS>
	class AddComponentToPolyhedron : public CGAL::Modifier_base<polyHDS> {
	private:
		Alphashape const *A;
		typename Alphashape::Facet f;
		bool addAllComponents;
		int numComp;
	public:
		AddComponentToPolyhedron(Alphashape const *A_,typename Alphashape::Facet const f_):A(A_),f(f_),addAllComponents(false),numComp(0){};
		AddComponentToPolyhedron(Alphashape const *A_):A(A_),f(std::pair<typename Alphashape::Cell_handle,int>()),addAllComponents(true),numComp(0){};
		virtual ~AddComponentToPolyhedron() {};
		int getNumComp(){return numComp;};
		void operator()(polyHDS &H){
			typedef CGAL::HalfedgeDS_default<typename polyHDS::Traits,CGAL::Polyhedron_items_3>	HDS;
			HDS h;
			AlphashapeHDSator<Alphashape, HDS> add(A);
			if (addAllComponents){
				numComp=add.addAllComponents(h);
			} else {
				numComp=add.addFacetComponent(f,h);
			}
			
			CGAL::Polyhedron_incremental_builder_3<polyHDS> B( H, true);
			B.begin_surface( h.size_of_vertices(),h.size_of_faces(),h.size_of_halfedges());
			std::map<typename HDS::Vertex*,int> vertIndex;
			int idx=0;
			for(typename HDS::Vertex_iterator it=h.vertices_begin();it!=h.vertices_end();++it){
	        	B.add_vertex( it->point() );
	        	vertIndex[&*it]=idx;++idx;
			}
			std::list<int> indices;
			for(typename HDS::Face_iterator it=h.faces_begin();it!=h.faces_end();++it){
				indices.clear();
				typename HDS::Halfedge_handle hh=it->halfedge();
				indices.push_back(vertIndex[&*(hh->vertex())]);
				indices.push_back(vertIndex[&*(hh->next()->vertex())]);
				indices.push_back(vertIndex[&*(hh->next()->next()->vertex())]);
				if (B.test_facet(indices.begin(),indices.end())) {
					B.add_facet(indices.begin(),indices.end());
				} else {
					B.rollback();
					numComp=0;
					return;
				}
			}
	        B.end_surface();
		};
		
	};
	
private : 
	Alphashape const &A;
	
public:
	/*!
	 * Constructor
	 * [in] A, a reference to the alphashape from which we want to construct polyhedrons
	 */
	AlphashapeHDSPolyhedronifikator(Alphashape const &A_):A(A_){};
	/*!
	 * Destructor
	 */
	virtual ~AlphashapeHDSPolyhedronifikator(){};
	/*!
	 * Adds the connected component of facet f to Polyhedron P
	 * [in] f a facet in the Alphashape A
	 * [in] P a Polyhedron
	 * [return] the number of components inserted in P (0 or 1)
	 * 
	 * if f is not on the border of the alphashape, the method inserts nothing in P and outputs 0
	 * if the connected component already exists in P, it is nevertheless added to P.
	 */
	int addComponent(typename Alphashape::Facet const& f, Polyhedron &P);
	/*
	 * Adds the all connected components of A to Polyhedron P
	 * [in] P a Polyhedron
	 * [return] the number of components that have been inserted
	 */
	int addAllComponents(Polyhedron &P);

};



template <class Alphashape, class Polyhedron>
int AlphashapeHDSPolyhedronifikator<Alphashape,Polyhedron>::addComponent(typename Alphashape::Facet const& f, Polyhedron &P){
	AddComponentToPolyhedron<typename Polyhedron::HalfedgeDS> add(&A,f);
	P.delegate(add);
	return add.getNumComp();
}

template <class Alphashape, class Polyhedron>
int AlphashapeHDSPolyhedronifikator<Alphashape,Polyhedron>::addAllComponents(Polyhedron &P){
	AddComponentToPolyhedron<typename Polyhedron::HalfedgeDS> add(&A);
	P.delegate(add);
	return add.getNumComp();
}



#endif /* ALPHASHAPEPOLYHEDRONIFIKATOR_H_ */