/*
 * BackwardChainer.h
 *
 * Copyright (C) 2014 Misgana Bayetta
 *
 * Author: Misgana Bayetta <misgana.bayetta@gmail.com>  October 2014
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
#ifndef BACKWARDCHAINER_H_
#define BACKWARDCHAINER_H_

#include <opencog/reasoning/RuleEngine/rule-engine-src/Rule.h>

#include "PLNCommons.h"

#define DEBUG 1

class BackwardChainerUTest;

namespace opencog {
    
/**
 * Backward chaining falls in to two cases
 *  1.Truth value query - Given a target atom whose truth value is not known and a pool of atoms,find a way
 *    to estimate the truth value of the target Atom,via combining the atoms in the pool  using the inference rule/
 *     eg. The target is "Do people breath"(InheritanceLink people breath)...the truth value of the target is estima
 *     ated via doing the inference "People are animals,animals breathe,therefore people breathe."
 *  2.Variable fulfillment query - Given a target Link(Atoms may be Nodes or Links) with one or more VariableAtoms
 *  among its targets,figure what atoms may be put in place of these VariableAtoms,so as to give the target Link a
 *  hight strength * confidence (i.e "hight truth value")
 *  eg. What breathes( InheritanceLink $X breath ) can be fulfilled by pattern matching
 *   whenever there are are multiple values to fill $X we will use fitness value measure to choose the best
 *  other compound example is what breathes and adds
 *  ANDLink
 *  	InheritanceLink $X Breath
 *  	InheritanceLink $X adds
 *
 *  Anatomy of a single inferene
 *  ============================
 *  A single inference step may be viewed as follows
 *  1.Choose inference Rule R and a tuple of Atoms that collectively match the input condition of the rule
 *  2.Apply choosen rule R to the chosen input Atoms
 *  3.Create an ExecutionLink recording the output found
 *  4.In addition to retaining this ExecutionLink in the Atomspace.also save the copy of it in the InferenceRepository(
 *  this is not needed for the very first implementation,but will be very useful once PLN is in regular use.)
 */

class BackwardChainer
{
    friend class ::BackwardChainerUTest;

public:
	BackwardChainer(AtomSpace* as, std::vector<Rule>);
	~BackwardChainer();

	void do_chain(Handle init_target);
	map<Handle, UnorderedHandleSet>& get_chaining_result();

	AtomSpace* _as;
	std::map<int, HandleSeq> _step_inference_map; //for holding inference history

private:
	map<Handle, UnorderedHandleSet> do_bc(Handle& htarget);
	map<Handle, UnorderedHandleSet> bc_rule(Rule& rule, const map<Handle, Handle>& mapping);

	std::vector<Rule> filter_rules(Handle htarget);
	HandleSeq filter_grounded_experssions(Handle htarget);

	bool unify(const Handle& htarget, const Handle& match, map<Handle, Handle>& output);

	map<Handle, HandleSeq> get_logical_link_premises_map(Handle& himplicant);

	map<Handle, UnorderedHandleSet> join_premise_vgrounding_maps(const Handle& connector,
			const map<Handle, map<Handle, UnorderedHandleSet> >& premise_var_grounding_map);

	Rule select_rule(const std::vector<Rule>& rules);

	UnorderedHandleSet chase_var_values(Handle& hvar, const vector<map<Handle, UnorderedHandleSet>>& inference_list, UnorderedHandleSet& results);
	map<Handle, UnorderedHandleSet> ground_target_vars(Handle& hgoal, vector<map<Handle, UnorderedHandleSet>>& var_grounding_map);

	void remove_generated_rules();

#if DEBUG
	void print_inference_list();
	void print_premise_var_ground_mapping(const map<Handle,map<Handle,HandleSeq>>&);
	void print_var_value(const map<Handle,HandleSeq>&);
#endif

	PLNCommons* _commons;
	map<Handle, UnorderedHandleSet> _chaining_result;

	// XXX TODO do we need to store the rule used too?
	vector<map<Handle, UnorderedHandleSet>> _inference_list;

	std::vector<Rule> _rules_set;

	// need to be removed at the end of the backward chaining process
    HandleSeq _bc_generated_rules;

	// XXX any additional link should be reflected in @method join_premise_vgrounding_maps
	vector<Type> _logical_link_types = { AND_LINK, OR_LINK };

};

} // namespace opencog

#endif /* BACKWARDCHAINER_H_ */
