Separation is potentially feasible even without external temperature inputs. However, conditions apply, and two points remain unverified.

## Principle: At least two equations are required for two unknowns

Let the temperature variations of the magnet and core be $\theta_m$ and $\theta_c$, respectively. At an operating point $\boldsymbol i$, the flux linkage deviation can be expressed as:

$$\Delta\boldsymbol\psi(\boldsymbol i)\approx \boldsymbol b_m(\boldsymbol i)\,\theta_m+\boldsymbol b_c(\boldsymbol i)\,\theta_c$$

where $\boldsymbol b_m$ and $\boldsymbol b_c$ denote the variation modes of the magnet and core, respectively. To separate the two temperatures, at least two such equations are required. There are three approaches to increasing the number of equations without external inputs:

| Approach | Requirements | Validity Condition | Drawbacks |
| --- | --- | --- | --- |
| A. Using the full voltage vector ($\psi_d$ and $\psi_q$ components) at a single point | Both voltage components | $\boldsymbol b_m$ and $\boldsymbol b_c$ are not parallel at that operating point | The component parallel to the current vector suffers from errors in $R_s$ and dead-time (Issue 2)—the exact reason the Q-method discards this component. |
| B. Using only Q across multiple operating points | Operating points varying over time | Temperatures remain nearly constant within the observation window, and the ratio $h_m/h_c$ differs sufficiently across points ($h_x=\boldsymbol i^\mathsf T\boldsymbol b_x$) | The condition number degrades if operating points are clustered. Model errors trade off between the two estimated temperatures (Issues 4 & 5). |
| C. Exploiting the difference in thermal time constants | Thermal model | The core responds rapidly, whereas the magnet responds slowly | Dependent on thermal model parameters and cooling conditions. |

Approach B is the most promising, as it identifies both unknowns while preserving the core advantage of the Q-method (robustness against $R_s$ and dead-time errors).

## Rationale Supporting the Feasibility of B (From Current Results)

* The magnet effect (resembling a current shift) appears prominently in $\psi_d$ within the low-to-medium current range.
* The core effect (uniform variation of $\psi_q$ in rossia) becomes significant in the saturation region where $i_q > 200\text{ A}$.
* Consequently, Q is expected to be dominated by the magnet effect under light loads and by the core effect under high loads. Because the ratio $h_m/h_c$ varies with the operating point, sufficient diversity for parameter identification in Approach B appears to exist.

## Two Unverified Points

1. **The actual profiles of the two modes**: In the current rossia dataset, the magnet and core share the same temperature, making it impossible to decouple $\boldsymbol b_m$ and $\boldsymbol b_c$. JMAG simulation cases with decoupled magnet and core temperatures are required.
2. **Necessity of separation**: The ultimate goal is the correction of $P'$. If the ratio of sensitivity between Q and $P'$ is identical across operating points for both modes, the correction can be achieved using a single lumped state, rendering separation unnecessary.

$$\frac{g_m(\boldsymbol i)}{h_m(\boldsymbol i)}\quad \text{and}\quad \frac{g_c(\boldsymbol i)}{h_c(\boldsymbol i)}\qquad (g_x=\boldsymbol t^\mathsf T\boldsymbol b_x)$$

The critical question is whether these two ratios coincide. If they diverge, failure to separate them will lead to estimation bias in $P'$.
