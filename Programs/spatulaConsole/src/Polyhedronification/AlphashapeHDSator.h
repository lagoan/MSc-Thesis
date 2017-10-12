#ifndef ALPHASHAPEHDSATOR_H_
#define ALPHASHAPEHDSATOR_H_
/* Benjamin SCHWARZ (not that proud though) 
 *   7 april 2009 - Strasbourg, after OTAN party
 */

#include <iostream>

#include <map>
#include <list>
#include <CGAL/HalfedgeDS_default.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_items_3.h>
/*! \class
 *  provides methods to add alphashape connected components to an existing HDS
 * 	 - addFacetComponent allows one to add the connected component of a specific facet to the HDS 
 *   - addAllComponents allows one to add every connected components to the HDS
 */ 
template <class Alphashape, class HDS>
class AlphashapeHDSator {
private:
	typedef typename HDS::Vertex_handle HVertex_handle;
	typedef typename HDS::Vertex HVertex;
	typedef typename HDS::Halfedge	Halfedge;
	typedef typename HDS::Halfedge_handle Halfedge_handle;
	typedef typename HDS::Face_handle HFacet_handle;
	typedef typename HDS::Face HFacet;
	
	typedef typename Alphashape::Facet	Facet;
	typedef typename Alphashape::Vertex_handle	Vertex_handle;
	typedef typename Alphashape::Cell_handle	Cell_handle;
	typedef typename Alphashape::Cell	Cell;
	
	class Ashape_facet_HDSbuild_info {
	private:
		Halfedge* halfedges[4];
	public:
		Ashape_facet_HDSbuild_info(){
			halfedges[0]=halfedges[1]=halfedges[2]=halfedges[3]=NULL;
		}
		bool is_linked(int edge){
			return (halfedges[edge] != NULL);
		}
		void link(int edge,Halfedge_handle h){
			halfedges[edge]=&(*h);
		}
		Halfedge_handle edge(int edge){
			return halfedges[edge];
		}
	};
	class Ashape_cell_HDSbuild_info {
	private:
		Ashape_facet_HDSbuild_info* fsi[4];		
	public:
		Ashape_cell_HDSbuild_info(){
			fsi[0]=fsi[1]=fsi[2]=fsi[3]=NULL;
		}
		virtual ~Ashape_cell_HDSbuild_info(){
			if (fsi[0] != NULL) delete fsi[0];
			if (fsi[1] != NULL) delete fsi[1];
			if (fsi[2] != NULL) delete fsi[2];
			if (fsi[3] != NULL) delete fsi[3];
		}
		bool tag_facet(int i){
			assert( (i<4) || (i>0) );
			bool tmp = (fsi[i]==NULL);
			if (tmp) fsi[i]=new(Ashape_facet_HDSbuild_info);
			return tmp;
		}
		bool is_taged_facet(int i){
			return (fsi[i]!=NULL);
		};
		Ashape_facet_HDSbuild_info &facet(int i){
			return *(fsi[i]);
		}
	};
	typedef std::map<Cell*,Ashape_cell_HDSbuild_info> CellInfoMap;
private:
	Alphashape const *A;
	AlphashapeHDSator(){};
public:
	AlphashapeHDSator(Alphashape const *A_):A(A_){}
	virtual ~AlphashapeHDSator(){};
private:
	void process_facet(
					   HDS &hds ,
					   std::list< std::pair<Halfedge_handle,Vertex_handle> >& points_per_halfedges ,
					   Cell_handle base_cell ,
					   int const base_facet_index ,
					   CGAL::Orientation orientation,
					   CellInfoMap& cellsI
					   ) const;
	int addFacetComponent_ (Facet const& f,
			 				HDS& hds, 
			 				CellInfoMap &cellsI
			 				) const;
	
public:
	
	/*! constructs f's connected component and adds it to the HDS
	 */
    int addFacetComponent (Facet const& f,HDS& hds) const;
    
    /*! constructs every connected component of the alphashape
     *  and insert it in the HDS
     */ 
    int addAllComponents (HDS& hds) const;
    
};




template <class Alphashape, class HDS>
int AlphashapeHDSator<Alphashape,HDS>::addAllComponents (HDS& hds) const
{
	//	std::cerr<<"addAllComponents "<<std::endl;
	int compNum=0;
	CellInfoMap cellsI;
	std::list<typename Alphashape::Facet> facets;
	std::list<typename Alphashape::Facet> crustFacets;
	A->get_alpha_shape_facets(std::back_inserter(facets),Alphashape::REGULAR);
	for (typename std::list<typename Alphashape::Facet>::iterator fit=facets.begin();fit!=facets.end();++fit) {
		if (A->classify(fit->first)==Alphashape::INTERIOR){
			crustFacets.push_back(A->mirror_facet(*fit));
		} else {
			crustFacets.push_back(*fit);
		}
	}
	facets.clear();
	A->get_alpha_shape_facets(std::back_inserter(facets),Alphashape::SINGULAR);
	for (typename std::list<typename Alphashape::Facet>::iterator fit=facets.begin();fit!=facets.end();++fit) {
		crustFacets.push_back(*fit);
		crustFacets.push_back(A->mirror_facet(*fit));
	}
	for (typename std::list<typename Alphashape::Facet>::iterator fit=crustFacets.begin();fit!=crustFacets.end();++fit) {
		if ((cellsI.find(&*(fit->first))==cellsI.end())	||
			(!cellsI[&*(fit->first)].is_taged_facet(fit->second))) {
			
			compNum+=this->addFacetComponent_(*fit,hds,cellsI);
		}
	}
	return compNum;
}


template <class Alphashape, class HDS>
int AlphashapeHDSator<Alphashape,HDS>::addFacetComponent (Facet const& f,HDS& hds) const
{
	//	std::cerr<<"addFacetComponent "<<std::endl;
	CellInfoMap cellsI;
	return ( addFacetComponent_(f,hds,cellsI) );
}

/*
 * 
 * 
 */
template <class Alphashape, class HDS>
int AlphashapeHDSator<Alphashape,HDS>::addFacetComponent_ (
														   Facet const& f,
														   HDS& hds,
														   CellInfoMap &cellsI
														   ) const
{
	
	Cell_handle base_cell,previous_cell; // will store the cells that are visited
	int base_facet_index,previous_facet_index; // will store the index of facets that are visited
	CGAL::Orientation orientation;
	std::list<Facet> facets;
	if ( (A->classify(f) != Alphashape::REGULAR) && (A->classify(f) != Alphashape::SINGULAR) ) {
		return 0;
	} 
	if (cellsI.find(&(*base_cell))!=cellsI.end()) {
		return 0;
	}
	
	
	base_cell=f.first;
	base_facet_index=f.second;
	if (A->classify( base_cell ) != Alphashape::EXTERIOR) {
		// the current cell is "full", we shall take the mirror one
		previous_cell=base_cell;
		previous_facet_index=base_facet_index;
		base_cell=previous_cell->neighbor(previous_facet_index);
		//			base_facet_index=previous_cell->mirror_index(previous_facet_index);
		base_facet_index=A->mirror_index(previous_cell,previous_facet_index);
	}
	// Orientation of the base facet in the cell
	if (A->is_infinite(base_cell)) {
		orientation = CGAL::orientation(
										base_cell->vertex(( base_facet_index + 1)&3 )->point(),
										base_cell->vertex(( base_facet_index + 3)&3 )->point(),
										base_cell->vertex(( base_facet_index + 2)&3 )->point(),
										//				base_cell->mirror_vertex(  base_facet_index  )->point());
										A->mirror_vertex(base_cell, base_facet_index  )->point());
	} else {
		orientation = CGAL::orientation(
										base_cell->vertex(( base_facet_index + 1)&3 )->point(),
										base_cell->vertex(( base_facet_index + 2)&3 )->point(),
										base_cell->vertex(( base_facet_index + 3)&3 )->point(),
										base_cell->vertex(  base_facet_index  )->point());
	}
	
	cellsI[&(*base_cell)].tag_facet(base_facet_index);
	std::list< std::pair<Halfedge_handle,Vertex_handle> > points_per_halfedges;
	process_facet(hds , points_per_halfedges , base_cell , base_facet_index , orientation, cellsI);
	for(typename std::list< std::pair<Halfedge_handle,Vertex_handle> >::iterator it=points_per_halfedges.begin();it!=points_per_halfedges.end();++it) {
		if ( (it->first)->vertex()==NULL ) {	
			HVertex_handle v=hds.vertices_push_back(HVertex(it->second->point()));
			v->set_halfedge(it->first);
			Halfedge_handle first=it->first;
			Halfedge_handle h=it->first;
			do {
				h->set_vertex(v);
				h=h->next()->opposite();
			} while (h!=first);
		}
	}
	return 1;
};


template <class Alphashape, class HDS>
void AlphashapeHDSator<Alphashape,HDS>::process_facet(
													  HDS &hds ,
													  std::list< std::pair<Halfedge_handle,Vertex_handle> >& points_per_halfedges ,
													  Cell_handle base_cell ,
													  int const base_facet_index ,
													  CGAL::Orientation orientation,
													  CellInfoMap &cellsI
													  ) const
{
	// necessary during the rotation process
	Cell_handle 		previous_cell,current_cell;
	int 				previous_facet_index,current_facet_index;
	
	// edges will be mapped to the index of the vertex in the facet that is not on the edge
	int edge_idx[3];
	edge_idx[0] = ((base_facet_index+1)&3); // remark: "&3" <=> "%4"
	edge_idx[1] = ((base_facet_index+2)&3);
	edge_idx[2] = ((base_facet_index+3)&3);
	// AROUND EACH EDGE THAT IS NOT ALREADY LINKED	
	for (int e=0;e<3;e++) {
		if( ! cellsI[&(*base_cell)].facet(base_facet_index).is_linked(edge_idx[e]) ) {
			// ROTATE
			// init with the opposite facet in the same cell
			current_cell				= base_cell;
			current_facet_index		= edge_idx[e];
			int top						= base_facet_index;	// the third vertex of facets that we shall trasverse, the one that is not on the edge
			// spin until a new alpha cell is found
			while (A->classify(current_cell,current_facet_index)==Alphashape::EXTERIOR) {
				previous_cell			= current_cell;
				previous_facet_index		= current_facet_index;
				current_cell				= previous_cell->neighbor(previous_facet_index);
				current_facet_index		= current_cell->index(previous_cell->vertex(top));
				//				top						= previous_cell->mirror_index(previous_facet_index);
				top						= A->mirror_index(previous_cell,previous_facet_index);
			}
			// the new alpha cell is ( current_cell , current_facet_index )
			// top is the vertex of the facet that is not on the edge
			// MARK THE NEW FACET AS VISITED (and record it's previous state)
			
			bool new_facet_was_untouched = cellsI[&(*current_cell)].tag_facet(current_facet_index);
			
			Halfedge_handle h=hds.edges_push_back(Halfedge(),Halfedge());
			
			cellsI[&(*base_cell)].facet(base_facet_index).link(edge_idx[e],h);
			cellsI[&*current_cell].facet(current_facet_index).link(top,h->opposite());
			if (orientation==CGAL::POSITIVE) {
				points_per_halfedges.push_back(std::pair<Halfedge_handle,Vertex_handle>(h,base_cell->vertex(edge_idx[(e+2)%3])));
				points_per_halfedges.push_back(std::pair<Halfedge_handle,Vertex_handle>(h->opposite(),base_cell->vertex(edge_idx[(e+1)%3])));
			} else {
				points_per_halfedges.push_back(std::pair<Halfedge_handle,Vertex_handle>(h,base_cell->vertex(edge_idx[(e+1)%3])));
				points_per_halfedges.push_back(std::pair<Halfedge_handle,Vertex_handle>(h->opposite(),base_cell->vertex(edge_idx[(e+2)%3])));
			}
			if (new_facet_was_untouched) {
				// let's see if the edge indexing is the same in the new facet
				// basically, we look at the indices of vertices (e+1) and (e+2) in the new facet
				// if their difference is +1, then the orientation is reversed
				// otherwise the orientation remains
				CGAL::Orientation new_orientation;
				int a = current_cell->index(base_cell->vertex(edge_idx[(e+2)%3]))-current_facet_index-1;
				a = (((a%4)<0)?(a%4+4):(a%4));
				int b = current_cell->index(base_cell->vertex(edge_idx[(e+1)%3]))-current_facet_index-1;
				b = (((b%4)<0)?(b%4+4):(b%4));
				int diff = a - b;
				if (((diff+3)%3)==1){// -1 = 2
					new_orientation=((orientation==CGAL::POSITIVE)?(CGAL::NEGATIVE):(CGAL::POSITIVE));
				} else {
					new_orientation=orientation;
					
				}
				
				
				process_facet(hds,points_per_halfedges,current_cell,current_facet_index, new_orientation,cellsI);
			}
		}
	}
	HFacet_handle f=hds.faces_push_back(HFacet());
	Halfedge_handle h1,h2,h3;
	h1 = cellsI[&(*base_cell)].facet(base_facet_index).edge((base_facet_index+1)&3);
	h2 = cellsI[&(*base_cell)].facet(base_facet_index).edge((base_facet_index+2)&3);
	h3 = cellsI[&(*base_cell)].facet(base_facet_index).edge((base_facet_index+3)&3);
    // We'd like that facet to be POSITIVE oriented, that is : the top of cell is on the positive side of facet
    if (orientation==CGAL::POSITIVE){
		h1->set_next(h2);h2->set_next(h3);h3->set_next(h1);
		h1->set_prev(h3);h2->set_prev(h1);h3->set_prev(h2);
    } else {
		h1->set_next(h3);h2->set_next(h1);h3->set_next(h2);
		h1->set_prev(h2);h2->set_prev(h3);h3->set_prev(h1);
    }
	h1->set_face(f);
	h2->set_face(f);
	h3->set_face(f);
	f->set_halfedge(h1);
}

#endif /* ALPHASHAPEHDSATOR_H_ */
