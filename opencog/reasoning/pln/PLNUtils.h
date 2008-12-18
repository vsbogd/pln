/*
 * Fri Feb 18 11:35:16 2005
 * Copyright (C) 2005  Ari A. Heljakka <heljakka at gmail.com>  / Novamente LLC
 * Copyright (C) 2008 by Singularity Institute for Artificial Intelligence
 * Written by Joel Pitt <joel@fruitionnz.com>
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef PLN_UTILS_H
#define PLN_UTILS_H

//#include <stdio.h>
//#include <stdlib.h>

#ifdef WIN32
	#pragma warning (disable: 4786)
#endif

#include <map>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <iostream>
#include <stack>
#include <math.h>

#include <boost/variant.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>

// AtomSpace 
#include <opencog/server/CogServer.h>
#include <opencog/atomspace/Atom.h>
#include <opencog/atomspace/ClassServer.h>
#include <opencog/atomspace/Link.h>
#include <opencog/atomspace/Node.h>
#include <opencog/atomspace/TLB.h>

// This is a 3rd party tree template library
#include <opencog/util/tree.h>
#include <opencog/atomspace/utils.h>

#include "PLN.h"

#include "AtomLookupProvider.h"
//#include "PLNatom.h"
//#include "AtomSpaceWrapper.h"

using namespace std;
using namespace opencog;

#undef STLhas
#define STLhas(container, entity) ((container).find(entity) != (container).end())
#define STLhas2(container, entity) (find((container).begin(), (container).end(), (entity)) != (container).end())

#define Btr boost::shared_ptr
#define DeclareBtr(__T, __varname) Btr< __T > __varname(new __T)

namespace haxx {
    const int STD_VARS = 100;
}

/// meta is a vtree wrapped in a boost::shared_ptr
typedef Btr<vtree> meta;
/// hpair is a pair of Handles
typedef pair<Handle,Handle> hpair;

/** 
 * vector2 appears to provide an easy way to instantiate vectors with 1-4 items.
 */
template<typename T>
class vector2 : public std::vector<T>
{
public:
	vector2(const T& arg1, const T& arg2, const T& arg3, const T& arg4)
	{ push_back(arg1); push_back(arg2); push_back(arg3); push_back(arg4); }

	vector2(const T& arg1, const T& arg2, const T& arg3)
	{ push_back(arg1); push_back(arg2); push_back(arg3); }

	vector2(const T& arg1, const T& arg2)
	{ push_back(arg1); push_back(arg2);  }

	vector2(const T& arg1)
	{ push_back(arg1);  }
};

// defines print levels
#define SILENT          0
#define SHY             1
#define NORMAL          2
#define LOCAL_DEBUG     3
#define VERBOSE         4
#define DEBUG           5
extern int currentDebugLevel;
// Cprintf for log level output
// TODO: replace with OpenCog Logger system
int cprintf(int debugLevel, const char *format, ...);

void ReasoningLog(int l, std::string m);
#define LOG(l, m) ReasoningLog(l,m)
#define SET_LOG(s) _LOGPOS = s; _LOG_FIXED = true
#define LET_LOG _LOG_FIXED = false;
#define LOG_STR (_LOG_FIXED ? _LOGPOS : ( std::string(__FILE__) + ":" + i2str(__LINE__) ))

// Print out trees
void rawPrint(tree<Vertex>& t, tree<Vertex>::iterator top, int _rloglevel);
void rawPrint(tree<Vertex>::iterator top, int _rloglevel);

// Make a string with count # of the char c
string repeatc(const char c, const int count);

namespace reasoning
{

// Check whether things are equivalent?
bool unifiesTo(const vtree& lhs, const vtree& rhs, map<Handle, vtree>& Lbindings,
        map<Handle, vtree>& Rbindings, bool allow_rhs_binding, Type VarType = FW_VARIABLE_NODE);
bool unifiesWithVariableChangeTo(const vtree & lhs_t, const vtree & rhs_t,
				map<Handle,Handle>& bindings);

typedef Btr< std::vector<Vertex> > VertexVector;
typedef Btr< std::set<Vertex> > VertexSet;

// What are PostConditions used for ???
template <class T>
class PostCondition
{
public:
	virtual bool operator()(const T& arg) const=0;
	virtual ~PostCondition();
};

class NoCondition : public PostCondition<Handle>
{
public:
	bool operator()(const Handle& arg) const
	{
		return true;
	}
};

// Not needed ???
#if 0

template<typename T>
class ObjectFactory 
{
protected:
	ObjectFactory() {}
	set<T*> objs;
public:
	T* New() {
		T* object = new T();
		objs.insert(object);
		return object;
	}
	~ObjectFactory()
	{
	  set<T*>::iterator s;
		for (std::set<T*>::iterator i = objs.begin(); i != objs.end(); i++)
		{
			delete *i;
		}
	}
};

template<typename T>
struct tuple2 { tuple2() : t1(0), t2(0) {} T t1; T t2; };

template<typename T> 
struct tuple3 : public tuple2<T>
{ tuple3() : t3(0) {} T t3; };

template<typename T>
std::vector<T*> NewCartesianProduct1( std::vector<T*>& lhs, const std::vector<T>& rhs, int dim)
{
	std::vector<T*> ret;

	for (int i = 0; i < lhs.size(); i++)
	{
		for (int j = 0; j < rhs.size(); j++)
		{
			T* entry = new T[dim];

			for (int k = 0; k < dim-1; k++)
				entry[k] = lhs[i][k];
			entry[dim-1] = rhs[j];

			ret.push_back(entry);
		}

		delete lhs[i];
	}

	return ret;
}

template<typename T>
std::vector<T*> NewCartesianProduct( std::vector<std::vector<T> >& matrix)
{
	std::vector<T*> ret;
	if (matrix.empty() || matrix[0].empty())
		return ret;

	assert(matrix.size() != 1);

	for (int j = 0; j < matrix[0].size(); j++)
	{
		T* entry = new T[1];
		entry[0] = matrix[0][j];
		ret.push_back(entry);
	}

	for (int i = 1; i < matrix.size(); i++)
	{
		ret = NewCartesianProduct1<T>(ret, matrix[i],i+1);
	}

	return ret;
}

#endif

typedef std::map<Handle, Handle> bindingsT;
typedef std::map<Handle, vtree>  bindingsVTreeT;

struct ModifiedVTree : public vtree
{
	Handle original_handle;

	ModifiedVTree()
	: original_handle((Handle)0)
	{}

	ModifiedVTree(const vtree& rhs, Handle _original_handle = Handle::UNDEFINED)
	: original_handle(_original_handle)
	{
		(*(vtree*)this) = rhs;
	}
};

struct ModifiedBoundVTree : public ModifiedVTree
{
	ModifiedBoundVTree(const ModifiedVTree& rhs)
	{
		(*(ModifiedVTree*)this) = rhs;
	}

	Btr<bindingsVTreeT> bindings;
};

class BoundVTree : public vtree
{
protected:
	vtree my_std_tree;
public:
	BoundVTree() {}
    
    // Joel: Had to add constructors to route to the parent
    // How this compiled in Novamente without them, I have no idea.
    BoundVTree(const vtree::iterator_base& i) : vtree(i) {}
    BoundVTree(const Vertex& v) : vtree(v) {}

	BoundVTree(const ModifiedVTree& rhs)
	{
		(*(ModifiedVTree*)this) = rhs;
	}

	BoundVTree(const vtree& rhs, Btr<bindingsT> _bindings = Btr<bindingsT>())
	: bindings(_bindings)
	{
		(*(vtree*)this) = rhs;
	}

	BoundVTree* Clone() const {
		return new BoundVTree(*this, this->bindings);
	}

	Btr<bindingsT> bindings;

	const vtree& getStdTree();
	unsigned long getFingerPrint();
	void createMyStdTree();
};

typedef Btr<BoundVTree> BBvtree;

template<typename TransformerT>
Btr<vtree> tree_transform(vtree& vt_const, TransformerT transformer)
{
	Btr<vtree> ret(new vtree(vt_const));
	for(vtree::iterator v = ret->begin(); v != ret->end(); v++)
		*v = transformer(*v);
//	for_each(ret->begin(), ret->end(), _1 = transformer(_1));

	return ret;
}

/*Btr<vtree> tree_transform(vtree& vt_const, TransformerT transformer)
{
	Btr<vtree> ret(new vtree(vt_const));
	//foreach(Vertex& v, *ret)
//		v = transformer(v);
	for_each(v.begin(), v.end(), _1 = transformer(_1));

	return ret;
}*/

// It doesn't look like mbegin and mend are used here...
// Check if they are used by external methods. ???
template<typename T2, typename T>
struct mapper
{
	T mbegin, mend;
	map<T2,T2> m;

	mapper(map<T2,T2>& _m, T _mbegin, T _mend)
        : mbegin(_mbegin), mend(_mend), m(_m) {}
	T2 operator()(const T2& key)
	{
		T i = m.find(key);
		return (i == m.end()) ? key : i->second;
	}
};

/**
We may have a case like:
First bindings:
64 => Britney2:ConceptNode (11) <1.000000, 0.000000>   [209]
65 => Osama2:ConceptNode (11) <1.000000, 0.000000>     [210]
88 => :EvaluationLink (77) <0.800000, 0.200000>        [212]
  friendOf:PredicateNode (26) <1.000000, 0.000000>     [203]
  :ListLink (32) <1.000000, 0.000000>  [211]
     Britney2:ConceptNode (11) <1.000000, 0.000000>    [209]
     Osama2:ConceptNode (11) <1.000000, 0.000000>      [210]
2nd bindings:
88 => :EvaluationLink (77) <1.000000, 0.000000>        [281]
  friendOf:PredicateNode (26) <1.000000, 0.000000>     [203]
  :ListLink (32) <1.000000, 0.000000>  [280]
     $AAAAAAAAD:FWVariableNode (146) <1.000000, 0.000000>      [264]
     $AAAAAAAAE:FWVariableNode (146) <1.000000, 0.000000>      [265]

which must be found consistent when these guys are combined!
*/

template<typename T>
Btr<map<Vertex, Vertex> > toVertexMap(T mbegin, T mend)
{
	Btr<map<Vertex, Vertex> > ret(new map<Vertex, Vertex>);
	for(T next = mbegin; next != mend; ++next)
	{
//		(*ret)[Vertex(next->first)];
//		Vertex vv(next->second);
//		(*ret)[Vertex(next->first)] = Vertex((Handle)0);
		(*ret)[Vertex(next->first)] = Vertex(next->second);
	}

	return ret;
}


template<typename T1, typename bindContainerIterT, typename TM>
bool consistent(TM& b1, TM& b2, bindContainerIterT b1start, bindContainerIterT b1end, bindContainerIterT b2start, bindContainerIterT b2end)
{
	assert(b1.begin() == b1start);

	for (bindContainerIterT	b = b2start;
		b!= b2end;
		b++)
	{
		bindContainerIterT bit;

		if ((bit = b1.find(b->first)) != b1end &&
			!(bit->second == b->second))
		{
			///The same var bound different way. First virtualize them:

			vtree binder1(make_vtree(v2h(b->second)));
			vtree binder2(make_vtree(v2h(bit->second)));

			/// Then apply all bindings on both sides to both, to "normalize away" dependencies

			Btr<vtree> binder1A(tree_transform(binder1,   mapper<T1, bindContainerIterT>(b1, b1.begin(), b1.end())));
			Btr<vtree> binder1B(tree_transform(*binder1A, mapper<T1, bindContainerIterT>(b2, b2start, b2end)));
			Btr<vtree> binder2A(tree_transform(binder2,   mapper<T1, bindContainerIterT>(b1, b1start, b1end)));
			Btr<vtree> binder2B(tree_transform(*binder2A, mapper<T1, bindContainerIterT>(b2, b2start, b2end)));

			return *binder2B == *binder1B; //Check if it's still inconsistent.
		}
	}

	return true;
}

void print_binding(pair<Handle, vtree> i);

struct PLNexception
{
  string msg;
  PLNexception(string _msg) : msg(_msg) {}
  const char* what() const { return msg.c_str(); }
};

template<typename T1, typename T2, typename T3>
void insert_with_consistency_check(std::map<T1, T2>& m, T3 rstart, T3 rend)
{
	///haxx::
	Btr<map<Vertex, Vertex> > mV(toVertexMap(m.begin(), m.end()));
	Btr<map<Vertex, Vertex> > rV(toVertexMap(rstart, rend));
	
	if (consistent<Vertex>(*mV, *rV, mV->begin(), mV->end(), rV->begin(), rV->end()))
		m.insert(rstart, rend);
	else
	{
/*		puts("First bindings:");
		for_each(m.begin(), m.end(), &print_binding);
		puts("2nd bindings:");
		for_each(rstart, rend, &print_binding);*/

		throw PLNexception("InconsistentBindingException");
	}
}

void insert_with_consistency_check_bindingsVTreeT(map<Handle, vtree>& m, map<Handle, vtree>::iterator rstart, map<Handle, vtree>::iterator rend);


typedef pair<std::string, Handle> hsubst;
struct iAtomSpaceWrapper;

bool within(float a, float b, float diff);
bool equal_vectors(Handle* lhs, int lhs_arity, Handle* rhs);

template<typename ATOM_REPRESENTATION_T>
struct weak_atom
{
	ATOM_REPRESENTATION_T value;
	Btr<bindingsT> bindings;

	ATOM_REPRESENTATION_T GetValue() const { return value; }
	
	weak_atom(	ATOM_REPRESENTATION_T _value,
				bindingsT* _bindings = NULL)
	: value(_value), bindings(_bindings) {}
		
	weak_atom(	ATOM_REPRESENTATION_T _value,
				Btr<bindingsT> _bindings)
	: value(_value), bindings(_bindings) {}
	weak_atom() : bindings(new bindingsT) {}
	~weak_atom() { }
	/// Shared ownership of bindings!
	weak_atom(const weak_atom& rhs)
	: value(rhs.value), bindings(rhs.bindings)
	{}
	/// Does not share ownership of bindings!!!
	weak_atom(const weak_atom& rhs, Btr<bindingsT> _bindings)
		: value(rhs.value)
	{
		this->bindings = Btr<bindingsT>(
			rhs.bindings ? new bindingsT(*rhs.bindings) : new bindingsT);
		insert_with_consistency_check(*this->bindings, _bindings->begin(), _bindings->end());
	}
	
	bool operator()(Handle h);
	bool operator<(const weak_atom<ATOM_REPRESENTATION_T>& rhs) const
	{
		return value < rhs.value;
	}
	void apply_bindings();	
};

typedef weak_atom<Vertex> BoundVertex;
typedef set<Vertex> BasicVertexSet;
typedef std::vector<reasoning::BoundVertex> BV_Vector;
typedef std::set<reasoning::BoundVertex> BV_Set;

struct TableGather : public std::set<weak_atom<Vertex> >
{
	TableGather(tree<Vertex>& _MP, AtomLookupProvider* aprovider=NULL, const Type VarT = FW_VARIABLE_NODE, int index=-1);
	void gather(tree<Vertex>& _MP, AtomLookupProvider* aprovider=NULL, const Type VarT = FW_VARIABLE_NODE, int index=-1);
};

bool HandleSeqHas(const std::vector<Handle>& container, Handle key);

bool getLargestIntersection(const set<Handle>& keyelem_set, const set<Handle>& link_set, Handle& result);

template<typename T>
bool vectorHas(std::vector<T> box, T key)
{
	for (uint i = 0; i < box.size(); i++)
		if (box[i] == key)
			return true;
	return false;
}

/*template<typename T>
void swap(T* a, T* b) { T temp = *a; *a = *b; *b = temp; }*/

void printTree(Handle h, int level = 0, int LogLevel = 5);
std::string GetRandomString(int size);

bool equal(Handle A, Handle B);

Handle satisfyingSet(Handle h);
HandleSeq constitutedSet(Handle P, float min_membershipStrength, float min_membershipCount);

template<typename T>
std::vector<T*> NewCartesianProduct( std::vector<std::vector<T> >& matrix);

struct atom;

void unifiesWithVariableChangeTo_TEST();
	
bool MPunifyHandle(Handle lhs,
				const atom& rhs,
				bindingsT& bindings,
				set<hsubst>** forbiddenBindings=NULL,
				bool* restart=NULL, const Type VarT = FW_VARIABLE_NODE);

bool MPunifyVector(tree<Vertex>& lhs_t, tree<Vertex>::iterator lhs_top, 
						const vector<Btr<atom> >& rhsv,
						bindingsT& bindings,
						set<hsubst>** forbiddenBindings=NULL,
						bool* restart=NULL, const Type VarT = FW_VARIABLE_NODE);

bool MPunify1(tree<Vertex>& lhs_t, tree<Vertex>::iterator lhs_ti,
				const atom& rhs,
				bindingsT& bindings,
				set<hsubst>** forbiddenBindings=NULL,
				bool* restart=NULL, const Type VarT = FW_VARIABLE_NODE);

template<typename OP1, typename OP2, typename ArgT, typename RetT>
class Concat
{
public:
	OP1 op1;
	OP2 op2;
	RetT operator()(ArgT& arg)
	{
		return op2(op1(arg));
	}
};

class GetHandle
{
public:
	GetHandle(){}
	Handle operator()(const Vertex& v) { return boost::get<Handle>(v); }
};

class DropVertexBindings
{
	public:
	Vertex operator()(const BoundVertex& rhs)
	{
		if (!rhs.bindings)
			return rhs.value;
		
		bindingsT::const_iterator i = rhs.bindings->find(boost::get<Handle>(rhs.value));
		
		/// The variable may be bound to another variable, so we have to call this recursively.
		
		return (i == rhs.bindings->end())
					? rhs.value
					: DropVertexBindings()(BoundVertex(i->second, rhs.bindings));
//					: Vertex(i->second);
	}
};

void convertTo(const vector<Vertex>& args, auto_ptr<Handle>& ret);
void convertTo(const VertexVector& args, auto_ptr<Handle>& ret);
void convertTo(const vector<BoundVertex>& args, auto_ptr<Handle>& ret);
void convertTo(const set<BoundVertex>& args, auto_ptr<Handle>& ret);
void convertTo(const VertexSet& args, auto_ptr<Handle>& ret);
void convertTo(const vector<Handle>& args, auto_ptr<Handle>& ret);
void convertTo(const VertexVector& args, Handle*& ret);
void convertTo(const vector<Vertex>& args, Handle*& ret);
void convertTo(const vector<BoundVertex>& args, Handle*& ret);
void convertTo(const vector<BoundVertex>& args, HandleSeq& ret);
void convertTo(const set<BoundVertex>& args, Handle*& ret);
void convertTo(const VertexSet& args, Handle*& ret);
void convertTo(const vector<Handle>& args, Handle*& ret);

template<typename T>
bool deref_equal(T a, T b) { return *a == *b; }

Handle _v2h(const Vertex& v);

struct getOutgoingFun : public binary_function<Handle, int, Handle>
{
    Handle operator()(Handle h, int i);
};

#define getTypeFun std::bind1st(std::mem_fun(&AtomSpaceWrapper::getType), GET_ATW)
//#define getOutgoingFun std::bind1st(std::mem_fun(&AtomSpaceWrapper::getOutgoing),GET_ATW)
#define getTypeVFun bind(getTypeFun, bind(&_v2h, _1))

#define getFW_VAR(vt) (find_if((vt).begin(), (vt).end(), \
			bind(equal_to<Type>(), \
				bind(getTypeFun, bind(&_v2h, _1)), \
				(Type)FW_VARIABLE_NODE )))

#define hasFW_VAR(vt) (getFW_VAR(vt)	!= (vt).end())

/// VariableNodes not memory-managed.
Vertex CreateVar(iAtomSpaceWrapper* atw, std::string varname);
Vertex CreateVar(iAtomSpaceWrapper* atw);

const char* Type2Name(Type t);
string condensed_form(const atom& a);

Btr< set<Btr<ModifiedBoundVTree> > > FindMatchingUniversals(Btr<vtree> target, iAtomSpaceWrapper* table);
Btr<ModifiedBoundVTree> FindMatchingUniversal(meta target, Handle ForAllLink, iAtomSpaceWrapper* table);

void bind(BoundVTree& bbvt, hpair new_bind);
meta bind_vtree(vtree &targ, const map<Handle, Handle>& binds);
void bind_Bvtree(meta arg, const bindingsVTreeT& binds);
void removeRecursionFromHandleHandleMap(bindingsT& ret_bindings);

void printBinding(const pair<const Handle,Handle> p);
bool equalVariableStructure(const vtree& lhs, const vtree& rhs);
bool equalVariableStructure2(BBvtree lhs, BBvtree rhs);

void printSubsts(BoundVertex a, int LogLevel);
Handle make_real(vtree& vt);
// #define fabs(a) (((a)>0.0f) ? (a) : (-a)) // now using math.h as this define clashes with headers

template<typename T, typename T2>
T2 second(const pair<T, T2>& p) { return p.second; }
template<typename T, typename T2>
T first(const pair<T, T2>& p) { return p.first; }

template<typename T, typename T2>
void removeRecursionFromMap(T mbegin, T mend)
{
	T mnext = mbegin;
cprintf(4,"removeRecursionFromMap...\n");
	while (mnext != mend)
	{
		T2 mnodenext = mnext->second.begin();

		while (mnodenext != mnext->second.end())
		{
			T next_mapping = find_if(mbegin, mend, bind(equal_to<Handle>(), v2h(*mnodenext), bind(&first<Handle, vtree>, _1)) ); //mbegin->second == _1);
		
			if (next_mapping != mend)
			{
				assert(next_mapping->first == v2h(*mnodenext));
				mnext->second.replace(mnodenext, next_mapping->second.begin());

				/// The replace call invalidates the iterator. Re-initialize:
				mnodenext = mnext->second.begin();
			}
			else
				mnodenext++;
		}
		mnext++;
	}
cprintf(4,"removeRecursionFromMap OK!\n");
}

#define NewNode(_T, _NAME) mva(atw->addNode(_T, _NAME, TruthValue::TRIVIAL_TV(), false,false))
#define makemeta(atom_description) meta(new tree<Vertex>(atom_description))

template<typename T1, typename T2>
bool overlap(T1 & abegin, T1& aend, T2& b)
{
	for (T1 ai = abegin; ai != aend; ai++)
		if (STLhas(b, *ai))
			return true;

	return false;
}

meta ForceAllLinksVirtual(meta target);
meta ForceRootLinkVirtual(meta target);

void print_progress();

bool RealHandle(meta _target, Btr<set<BoundVertex> > result_set);

template<typename MapIteratorT, typename MapItemT>
void removeRecursionFromMapSimple(MapIteratorT mbegin, MapIteratorT mend)
{
cprintf(4,"removeRecursionFromMap...\n");

	MapIteratorT mnext = mbegin, next_mapping;

	while (mnext != mend)
	{
		if ( (next_mapping = find_if(mbegin, mend, bind(equal_to<MapItemT>(), mnext->second, bind(&first<MapItemT, MapItemT>, _1))))
			!= mend)
		{
			assert(next_mapping->first == mnext->second);
			mnext->second = next_mapping->second;
		}
		else
			mnext++;
	}

cprintf(4,"removeRecursionFromMap OK!\n");
}

void makeHandletree(Handle real, bool fullVirtual, tree<Vertex>& ret);

bool substitutableTo(Handle from,Handle to,
						map<Handle,Handle>& bindings);

bool IsIdenticalHigherConfidenceAtom(Handle a, Handle b);
} // namespace reasoning


const int __LLEVEL = 4;
#if 0
template<typename VectorT>
bool consistentBindings(const VectorT& argVectorCandidate)
{
		reasoning::bindingsT ok_set;
		for (std::vector<reasoning::BoundVertex>::const_iterator	av = argVectorCandidate.begin();
											av!= argVectorCandidate.end();
											av++)
		{
			if (av->bindings)
			{
				for (reasoning::bindingsT::const_iterator b = av->bindings->begin();
												 b!= av->bindings->end();
												 b++)
				{
/*					LOG(__LLEVEL, "Next binding:");
					printBinding(*b);
printTree(b->second,0,3);*/
					reasoning::bindingsT::const_iterator b2;
			
					if ((b2 = ok_set.find(b->first)) == ok_set.end()) //if the var so far unbound
					{
//						LOG(__LLEVEL,"new consistent");
						ok_set.insert(*b); //then we'll pass it on...
					}
					else if (b2->second != b->second) //if the same var bound different way					
					{
//						LOG(__LLEVEL,"inconsistent");
						return false; //it's inconsistency
					}
				}
			}
		}
		
		return true;
	}
#endif
/*template<typename T, typename InputIterT>
struct _cC_op
{
	_cC_op(	InputIterT input_vector_begin,
			InputIterT input_vector_end,
			InputIterT this_arg_number)
			: 

	void operator(T i)
	{
LOG(__LLEVEL,"this_arg_number not empty...");		
				if (i.bindings)
					printf(4, "HAS BINDINGS! %d\n", i.bindings->size());
				else
					printf(4, "HAS NO BINDINGS!\n");
				
				VectorT next_head = head;
				next_head.push_back(i);
printf(__LLEVEL,"(*i) ok, next_head.size=%d\n", next_head.size());
				if (consistentBindings(next_head))
				{
LOG(__LLEVEL,"existsConsistentBinding = true");
					std::vector<Btr<SetT> >::const_iterator temp_i = this_arg_number;
					if (++temp_i == input_vector_end)
						existsConsistentBinding = true;
				}
				else
					return;
LOG(__LLEVEL,"createCombinations AGAIN...");
				createCombinations(	next_head,
					input_vector_begin,
					input_vector_end,
					this_arg_number+1,
					output_set);
	}
};*/

template<typename SetIterT, typename VectorT, typename InputIterT, typename OutputIterT>
void createCombinations(	const VectorT& head,
							//const std::vector<Btr<SetT> >& input_vector,
							InputIterT input_vector_begin,
							InputIterT input_vector_end,
							//std::vector<Btr<SetT> >::const_iterator this_arg_number,
							InputIterT this_arg_number,
							SetIterT this_arg_number_begin,
							SetIterT this_arg_number_end,
							OutputIterT& output_iter)
	{
		if (this_arg_number != input_vector_end)
		{
			bool existsConsistentBinding = false;
			assert(*this_arg_number);

//			for_each((*this_arg_number)->begin(), (*this_arg_number)->end(), _cC_op());

//			for (InputIterIterT	i = (*this_arg_number)->begin();
//								i!= (*this_arg_number)->end(); i++)
			for (SetIterT	i = this_arg_number_begin;
							i!= this_arg_number_end; i++)
			{
				VectorT next_head(head);
				next_head.push_back(*i);
//cprintf(__LLEVEL,"(*i) ok, next_head.size=%d\n", next_head.size());
//				if (consistentBindings(next_head))
				if (true)
				{
					InputIterT temp_i = this_arg_number;
					if (++temp_i == input_vector_end)
						existsConsistentBinding = true;
				}
				else
					continue;

				
				InputIterT next_arg_number = this_arg_number +
					((this_arg_number+1 != input_vector_end)
					? 1
					: 0);

					createCombinations(
						next_head,
						input_vector_begin,
						input_vector_end,
						this_arg_number+1,
						(*next_arg_number)->begin(),
						(*next_arg_number)->end(),
						output_iter);
			}		
			if (!existsConsistentBinding)
				return;
//				throw std::string("No consistent binding of Rule input std::vector elements was found!");
		}
		else
		{
			*(output_iter++) = head;
		}
	}

void removeRecursion(std::vector<Btr<reasoning::BoundVertex> >& multi_input_vector);


template<typename VectorT, typename InputIterT, typename OutputIterT>
void expandVectorSet(	InputIterT multi_input_vector_begin,
						InputIterT multi_input_vector_end,
						OutputIterT output_iter)
{

/*		for (uint k=0;k< multi_input_vector.size(); k++)
			for (std::set<reasoning::BoundVertex>::iterator s=multi_input_vector[k]->begin();
															s!=multi_input_vector[k]->end();s++)
				if (s->bindings)*/

		VectorT empty_head;

		try
		{
			createCombinations(	empty_head,
								multi_input_vector_begin,
								multi_input_vector_end,
								multi_input_vector_begin,
								(*multi_input_vector_begin)->begin(),
								(*multi_input_vector_begin)->end(),
								output_iter);
/*LOG(__LLEVEL, "Combinations:-------\n");
			for (set<VectorT>::iterator i = output_set.begin();
											i!= output_set.end();
											i++)
			{
				LOG(__LLEVEL, "\nNext vector:\n");
				for (uint j=0; j<i->size(); j++)
				{
					const Handle* h_ptr = v2h(&((*i)[j].value));
					if (!h_ptr)
					{	LOG(__LLEVEL,"(non-Handle)"); }
					else	
						cprintf(__LLEVEL,"[%d]\n", (int)*h_ptr);
						//printTree(*h_ptr,0,3);		
					
					if ((*i)[j].bindings)
						cprintf(__LLEVEL, "HAS BINDINGS %d!\n",(*i)[j].bindings->size());
				}
				LOG(__LLEVEL, "XXXXXXXXXXXXXXXx-------\n");
			}*/

		} catch(std::string s) { 
			puts(s.c_str()); 
		}
	}

#define vt2h(vtreeprovider) v2h(*(vtreeprovider).getVtree().begin())

#endif
