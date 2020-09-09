#!python
#cython: language_level=3
"""An implementation and demonstration of Bayesian Networks in Cython.
"""

from cygraph.graph_ cimport DynamicGraph


cdef class BayesianNetwork:
    """A bayesian network class capable of inneficient inference using
    the enumeration algorithm. Only capable of handling binary
    variables.

    Parameters
    ----------
    variables: list of tuples
        A list of variable names, their probabilities of being true, and
        descriptions of their meaning.
        Assumes tuples are (str, float, str).
    """

    cdef DynamicGraph graph

    def __cinit__(self, list variables=[]):
        self.graph = DynamicGraph(graph=None, directed=True,
            vertices=variables)

    cpdef void add_edge(self, str A, str B, float conditional_probability
            ) except *:
        """Adds an edge to the network, representing the conditional
        proability between the two variables.

        Parameters
        ----------
        A: str
            The causing variable (parent).
        B: str
            The dependent variable (child).
        conditional_probability: float
            The probability that event A occurs given that event B
            occurs. P(A | B)
        """
        self.graph.add_edge(A, B, conditional_probability)

    cpdef void add_vertex(self, str name, float prior_probability) except *:
        """Adds a vertex to the network, representing a new variable.

        Parameters
        ----------
        name: str
            The name of the variable.
        prior_probability: float
            The probability that the event represented by this variable
            will occur.
        """
        self.graph.add_vertex(name)
        self.graph.set_vertex_attribute(name, key="Prior Probability",
            val=prior_probability)

    cpdef float get_joint_probability(self, list false_vars=[]) except *:
        """Gets the joint probability of all the nodes in the network.

        Parameters
        ----------
        false_vars: list of str
            Variables that are treated as false instead of true (so that
            their prior probability is one minus what was inputted when
            they were added to the network.)

        Returns
        -------
        float
            The joint probability of all of the variables in the network
            being either true or false (depending on false_vars).
        """
        cdef str variable, parent
        cdef set parents
        cdef float variable_probability, conditional_probability
        cdef bint false_var

        cdef float joint_probability = 1.0

        for variable in self.graph.vertices:
            variable_probability = 1.0

            false_var = variable in false_vars
            parents = self.graph.get_parents(variable)
            if parents:
                # Use conditional probability.
                for parent in parents:
                    conditional_probability = self.graph.get_edge_weight(
                        parent, variable)
                    variable_probability *= (
                        1 - conditional_probability if false_var
                        else conditional_probability)
            else:
                # Use prior probability.
                prior_probability = self.graph.get_vertex_attribute(
                    variable, "Prior Probability")
                variable_probability = (1 - prior_probability if false_var
                    else prior_probability)

            joint_probability *= variable_probability

        return joint_probability

    cpdef float marginalize_joint_probability(self, list nuisance_variables
            ) except *:
        """Finds the joint probability of the network after summing out
        the nuisance variables.

        Parameters
        ----------
        nuisance_variables: list
            A list of variables to exclude from the joint probability
            using marginalization.
        """
        cdef int i
        cdef str var
        cdef bint state, visited
        cdef list var_states
        cdef float probability_sum = 0.0
        cdef int n_nuisance_variables = len(nuisance_variables)

        cdef list stack = [[]]
        while stack:
            var_states = stack.pop()
            if len(var_states) == n_nuisance_variables:
                # Base case: The code inside the final for-loop in the
                # nested for-loops of enumeration.
                probability_sum += self.get_joint_probability(
                    [var for var, state in zip(nuisance_variables, var_states)
                     if not state])
            else:
                for state in [True, False]:
                    stack.append(var_states + [state])

        return probability_sum

    cpdef float get_conditional_probability(self, str A, str B
            ) except *:
        """Calculates the conditional probability of a node being true given
        any other node.

        Parameters
        ----------
        A: str
            The causing variable (parent).
        B: str
            The dependent variable (child).

        Returns
        -------
        float
            The probability that event A occurs given that event B
            occurs. In mathematical notation, P(A | B)
        """
        # Calculate joint probability of distribution with only the
        # relevant variables.
        cdef str var
        cdef list nuisance_variables = [var for var in self.graph.vertices
                                        if var not in (A, B)]
        cdef float marginalized_joint_probability = \
            self.marginalize_joint_probability(nuisance_variables)

        return (marginalized_joint_probability
              / self.graph.get_vertex_attribute(B, "Prior Probability"))


# Example from 3Blue1Brown video "Bayes theorem".
bayesian_network = BayesianNetwork()
# 1/21 chance Steve is a librarian.
bayesian_network.add_vertex('L', 1 / 21)
# 24 / 210 change Steve is shy.
bayesian_network.add_vertex('S', 24 / 210)
# 10% change Steve only wears white shirts.
bayesian_network.add_vertex('W', 0.1)
# 40% chance Steve is shy given that he is a librarian.
bayesian_network.add_edge('L', 'S', 0.4)

cond_prob_1 = bayesian_network.get_conditional_probability('L', 'S')
print("The probability that Steve is a librarian given that he is shy: "
     f"{cond_prob_1}")