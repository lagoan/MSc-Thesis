#ifndef ALPHASHAPEPOLYHEDRONIFIKATOR_H_
#define ALPHASHAPEPOLYHEDRONIFIKATOR_H_
/* Benjamin SCHWARZ (not that proud though) 
 *   7 april 2009 - Strasbourg, after OTAN party
 */
#include <map>
#include <list>
#include <CGAL/Polyhedron_incremental_builder_3.h>

template <class Alphashape, class Polyhedron>
class AlphashapePolyhedronifikator {
private :
	/*! inner class providing the operator() which will be delegated
	 * the construction of the polyhedron
	 */
	template <class PolyHDS>
		class AddComponentToPolyhedron : public CGAL::Modifier_base<PolyHDS> {
		private :
			//! a reference to the alpha shape of wich we want to extract the border
			Alphashape const &A;
			//! a tag to register if last operation was ok, if not, no polyhedron has been built. 
			bool lastCallWasOK;
		public  :
			//! constructor
			AddComponentToPolyhedron(Alphashape const &A_):A(A_),lastCallWasOK(false){};
			//! destructor
			~AddComponentToPolyhedron(){};
			//! sis the last call to operator() result in the creation of a polyhedron ?
			bool polyhedronWasInserted(){return lastCallWasOK;}
			//! delegated operator() for the construction of a polyhedron
			void operator()(PolyHDS& H) {
				// get alpha shape regular vertices and facets
				std::list<typename Alphashape::Vertex_handle> vertices;
				A.get_alpha_shape_vertices(std::back_inserter(vertices),Alphashape::REGULAR);
				std::list<typename Alphashape::Facet> facets;
				A.get_alpha_shape_facets(std::back_inserter(facets),Alphashape::REGULAR);
				
				// init the surface construction
				CGAL::Polyhedron_incremental_builder_3<PolyHDS> B( H, true);
				B.begin_surface( vertices.size(), facets.size());
				
				// register vertices by there memory addr, and insert them in the polyhedron
				std::map<typename Alphashape::Vertex const*,int> vertIdx;
				int idx=0;
				for (typename std::list<typename Alphashape::Vertex_handle>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
				{
					vertIdx[&**it]=idx++;
					B.add_vertex((*it)->point());
				}

				// insert facets
				std::list<int> indices; // will host vertex indices of the facet vertices
				for (typename std::list<typename Alphashape::Facet>::iterator it=facets.begin(); it!=facets.end(); ++it)
				{
					if (A.classify(it->first)==Alphashape::EXTERIOR) {
						typename Alphashape::Cell_handle c=it->first->neighbor(it->second);
						it->second = c->index(it->first);
						it->first = c;
					}
					indices.clear();
					for (int k=0;k<3;k++){
						indices.push_back(vertIdx[&*(it->first->vertex(Alphashape::Triangulation_utils_3::vertex_triple_index(it->second,k)))]);
					}
					if (B.test_facet(indices.begin(),indices.end())) {
						B.add_facet(indices.begin(),indices.end());
					} else {
						B.rollback();
						return;
					}
				}
				//! if we are here, things might have gone well...
				lastCallWasOK=true;
				B.end_surface();
			};
		};
private :
	Alphashape const&A;
	
public :
	/*! constructor
	 */
	AlphashapePolyhedronifikator( Alphashape const &A_ ): A(A_){}
	/*! destructor
	 */
	virtual ~AlphashapePolyhedronifikator(){}
	/*! transforms the border of the alpha shape A to a polyhedron
	 * [in] P a reference to a polyhedron object that we will fill
	 * [return] true if the polyhedron has been inserted, 
	 *          false if there was a problem during the construction (P is unchanged)
	 */
	bool addAllComponents(Polyhedron &P);
};

template <class Alphashape, class Polyhedron>
bool AlphashapePolyhedronifikator<Alphashape,Polyhedron>::addAllComponents(Polyhedron &P) {
	AddComponentToPolyhedron<typename Polyhedron::HalfedgeDS> add(A);
	P.delegate(add);
	return add.polyhedronWasInserted();
}

#endif /*ALPHASHAPEPOLYHEDRONIFIKATOR_H_*/
