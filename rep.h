# Temperature-Change Patterns of the rossia Flux Maps: Current Shift, Fixed Mode and PM-Flux Offset (Report 3, v5)

> Status: complete (2026-10-08). Exploratory analysis; no pass criteria were set.
> Analysis code: `src/r3_rossia_current_shift.py` (reuses the v4 helpers in `src/r3_bio_current_shift.py`)
> Figures and summary: `report/figures/qw2p3_rossia_v5/`
> Preceding report: [Report 3 v4 (bio, Japanese)](qw2p3_report_v4.md)

## 0. Position of this report

Report 3 v4 tested whether the PM-as-current-source model of Srinivasan et al. explains the temperature dependence of the bio flux maps. A scalar d-axis current shift left about 25 % residual, mainly in the q-axis component. A fixed temperature mode left 1.4–3.4 %. This report repeats the same three patterns on rossia. rossia has four temperatures and two independent map fits (TPS and measured co-energy). The four temperatures also allow a test that bio could not support: predicting an unused temperature from a coefficient determined at another temperature.

The analysis was done before a plan document was written. The questions below restate what the analysis set out to examine; they were not fixed in advance. The scaling diagnostic in §4.5 and the interpretation in §5 were added after the results were seen.

Out of scope: estimator implementation, online tracking, real-machine validity, and separating the magnet and core contributions. §7 discusses the last item as a next step.

## 1. Background and questions

### 1.1 Technical issue

The temperature-induced flux change differs in direction and magnitude across the current plane. The estimator needs a compact description of this change with as few unknown states as possible. The fixed temperature mode needs a full second map for its shape. The current-shift model needs only one reference map and one scalar per temperature, provided that its premises hold.

The rossia flux maps come from a JMAG finite-element analysis converted into a JMAG-RT model. In each temperature case the whole motor, magnets and stator core, is held at the same temperature. The rossia stator core material has temperature-dependent magnetic properties. In this data set, magnet and core temperatures therefore always change together.

### 1.2 Questions

**Q1: How much of the rossia temperature change can each of the three patterns represent when one coefficient per temperature is fitted?**

**Q2: Can a coefficient determined at one temperature, scaled linearly with temperature, predict an unused temperature?**

**Q3: How do the residuals project onto the estimator observables $E_Q$ and $E_m$?**

**Q4: Do the conclusions depend on the map-fitting method (TPS or measured co-energy)?**

## 2. Models

With reference temperature $T_r = 75$ °C and $\Delta\boldsymbol\psi_T=\boldsymbol\psi_T-\boldsymbol\psi_{T_r}$, the patterns are as follows.

| Pattern | Model | Coefficient per temperature | Source of the shape |
|---|---|---|---|
| Current shift | $\hat{\boldsymbol\psi}_T(i_d,i_q)=\boldsymbol\psi_{T_r}(i_d+\Delta I_T,\,i_q)$ | $\Delta I_T$ [A] | structure of the reference map |
| Linearised shift (auxiliary) | $\boldsymbol\psi_{T_r}+\Delta I_T\,\partial\boldsymbol\psi_{T_r}/\partial i_d$ | $\Delta I_T$ | d-axis differential inductance column |
| Fixed mode | $\boldsymbol\psi_{T_r}+a_T\,\Delta\boldsymbol\psi_{S}$ | $a_T$ | difference map of a source temperature $S$ |
| PM-flux offset | $\boldsymbol\psi_{T_r}+(\Delta\Lambda_T,\,0)$ | $\Delta\Lambda_T$ [Wb] | uniform d-axis offset |

The current-shift model follows from a one-coil magnetic circuit with a linear magnet and a soft-iron reluctance that does **not** depend on temperature. Temperature then enters only through the magnet current $I_{pm}\propto B_r$. The model predicts that one scalar $\Delta I_T$, linear in $T$, reproduces both components of $\Delta\boldsymbol\psi_T$.

For the fixed mode, the source $S$ is −40 °C for the 25 °C and 150 °C targets, and 150 °C for the −40 °C target.

## 3. Method

### 3.1 Input data and map generation

| Item | Content |
|---|---|
| Source | Four VI files (−40/25/75/150 °C, 230 rows each, identical PTN command pattern) from the rossia JMAG-RT model, 1000 rpm, Vdc 1492 V |
| Cleaning | Rows were removed, never repaired. The rules covered: command agreement with the majority of the four temperatures; actual vs command current within 0.3 A + 0.1 %; current and voltage magnitudes vs the monitor columns; Vd/Vq consistency with the line through the other three temperatures; duplicates; and nonzero-Iq rows without a unique mirror partner |
| Kept rows (defective rows removed) | −40 °C: 212 (2), 25 °C: 184 (16), 75 °C: 167 (32), 150 °C: 204 (7) |
| Identified flux points | 112 / 98 / 89 / 108; Rs identified per file from mirror pairs |
| Map fits | `jp_model` TPS and measured co-energy; i_d −1100…1100 A × i_q 0…1100 A, 12.5 A grid; reference current 1060.015 A and domain 1100 A, the same as the existing 75 °C rossia asset |
| Fit diagnostics | Measured co-energy CV 3.2–4.1 mWb. TPS CV 1.9–2.3 mWb (d) except 7.4 mWb at 75 °C, where 26 command points are missing, including the i_d = +721 A edge. Measured co-energy Hessian-nonpositive nodes: 5/7/14/55 (−40/25/75/150 °C) |

The cleaning rules and per-row reasons are kept in `calibration_tools/jp_model/runcase/rossia_vi_cleaning/`, which is outside Git. The 25 °C file was used under its file label; an earlier study of the same motor used a 23 °C case.

### 3.2 Evaluation

- **Nodes**: inside the measured support at all four temperatures, the same for both fits (11,280 nodes; 11,273 for 150 °C after the shift). Unlike bio, the region includes positive $i_d$.
- **Relative residual**: $\lVert\boldsymbol\psi_T-\hat{\boldsymbol\psi}_T\rVert_2/\lVert\Delta\boldsymbol\psi_T\rVert_2$ over the concatenated nodes, the same definition as v4. Component residuals are normalised by the norm of the same component of $\Delta\boldsymbol\psi_T$.
- **Shift**: PCHIP interpolation along $i_d$, with no extrapolation. $\Delta I_T$ is found by a 1 A grid search over ±250 A followed by bounded refinement.
- **Interpolation floor**: every second $i_d$ column is rebuilt from its neighbours at 25 A spacing. The resulting relative error is 0.04–0.17 % of $\lVert\Delta\boldsymbol\psi_T\rVert$.
- **Prediction (Q2)**: $\Delta I_T=k(T-T_r)$ with $k=\Delta I_S/(S-T_r)$; fixed-mode coefficient $a_T=(T-T_r)/(S-T_r)$; PM-flux offset scaled in the same way. No data from the target temperature are used.
- **Projections (Q3)**: $E_Q=\boldsymbol i^\mathsf T\Delta\boldsymbol\psi$ and $E_m=i_q\Delta\psi_d-i_d\Delta\psi_q$ [J], computed on the residual of each model.

## 4. Results

### 4.1 Q1: fitted representation

**Table 1. Relative residual, one coefficient fitted per temperature (measured co-energy; TPS in parentheses).**

| Target | Current shift | Linearised shift | PM-flux offset | Fixed mode |
|---|---|---|---|---|
| −40 °C | 85.7 % (86.2) | 85.1 % (85.6) | 78.0 % (78.2) | 14.1 % (13.3) |
| 25 °C | 88.0 % (88.7) | 87.7 % (88.4) | 81.7 % (81.9) | 7.7 % (7.3) |
| 150 °C | 89.9 % (89.9) | 90.2 % (90.2) | 84.5 % (84.4) | 14.1 % (13.3) |

**Table 2. Component residuals, d / q (measured co-energy).**

| Target | Current shift | PM-flux offset | Fixed mode |
|---|---|---|---|
| −40 °C | 60.7 % / 105.0 % | 46.8 % / 100 % | 17.0 % / 10.3 % |
| 25 °C | 64.5 % / 103.9 % | 52.6 % / 100 % | 9.3 % / 5.9 % |
| 150 °C | 67.9 % / 103.4 % | 57.9 % / 100 % | 17.4 % / 10.9 % |

- The temperature change has q and d components of similar size. The RMS of $\Delta\psi_d$ / $\Delta\psi_q$ (nodes with $i_q>0$) is 22.1 / 22.1 mWb at −40 °C, 10.2 / 11.0 mWb at 25 °C and 15.3 / 17.6 mWb at 150 °C.
- $\Delta\psi_q$ is positive when cold (up to about +30 mWb at −40 °C) and negative when hot. It is nearly uniform over $i_q \gtrsim 200$ A (Fig. 2). The current shift instead predicts a pattern that changes sign across $i_d$. Its sign agrees with the actual $\Delta\psi_q$ at only 38–41 % of the nodes (correlation 0.31–0.36).
- Even in the d component, the shift-predicted change is uncorrelated with the actual change (correlation −0.03 to 0.00), and its d residual (61–68 %) exceeds that of the PM-flux offset (47–58 %).
- The fitted shifts are $\Delta I$ = +38.6 / +17.3 / −25.3 A (measured co-energy) and +38.1 / +16.8 / −25.1 A (TPS) for −40 / 25 / 150 °C.

![Residual magnitude, measured co-energy](figures/qw2p3_rossia_v5/residual_maps_measured_coenergy.png)

**Fig. 1.** Residual magnitude per node (measured co-energy, reference 75 °C). Columns: current shift, fixed mode, PM-flux offset. The maximum fixed-mode residual is 9.9 / 2.8 / 7.2 mWb (−40 / 25 / 150 °C), against 17–38 mWb for the other two patterns. The TPS version is `figures/qw2p3_rossia_v5/residual_maps_tps.png`.

![q-axis change, measured co-energy](figures/qw2p3_rossia_v5/q_change_measured_coenergy.png)

**Fig. 2.** $\Delta\psi_q$ relative to 75 °C. Top: actual. Bottom: predicted by the fitted current shift. The TPS version is `figures/qw2p3_rossia_v5/q_change_tps.png`.

### 4.2 Q2: prediction of an unused temperature

**Table 3. Relative residual when the coefficient comes from another temperature (measured co-energy; TPS in parentheses).**

| Source → target | Current shift | PM-flux offset | Fixed mode (linear in $T$) | Fixed mode, fitted (Table 1) |
|---|---|---|---|---|
| −40 → 25 °C | 88.0 % (88.7) | 81.7 % (81.9) | 12.0 % (12.0) | 7.7 % |
| −40 → 150 °C | 89.9 % (89.9) | 84.5 % (84.4) | 18.1 % (17.2) | 14.1 % |
| 150 → −40 °C | 85.7 % (86.2) | 78.0 % (78.3) | 20.7 % (19.4) | 14.1 % |
| 150 → 25 °C | 88.0 % (88.7) | 81.7 % (81.9) | 11.3 % (9.0) | 7.7 % |

- **The shift is linear in temperature.** $k$ = −0.336 / −0.337 A/K (measured co-energy, from −40 / 150 °C) and −0.331 / −0.335 A/K (TPS). Predicted and fitted $\Delta I$ agree within 0.5 A. The current at which $\psi_d=0$ on the $i_q=0$ row is 230 / 210 / 194 / 173 A (measured co-energy; TPS 226 / 205 / 189 / 168 A) for −40 / 25 / 75 / 150 °C, about −0.16 %/K relative to 75 °C.
- **The fixed-mode coefficient is not linear in temperature.** With the −40 °C shape, the fitted coefficients are 0.479 (25 °C) and −0.737 (150 °C), against 0.435 and −0.652 from linear scaling. Linear scaling therefore adds 3.6–6.6 points of residual at an unused temperature.

### 4.3 Q3: projections onto the estimator observables

**Table 4. RMS of $E_Q$ / $E_m$ residuals [J] (measured co-energy).**

| Target | No correction | Current shift | PM-flux offset | Fixed mode |
|---|---|---|---|---|
| −40 °C | 18.95 / 14.07 | 19.52 / 7.18 | 18.38 / 7.51 | 2.27 / 1.97 |
| 25 °C | 9.42 / 6.43 | 9.72 / 3.44 | 9.25 / 3.64 | 0.64 / 0.53 |
| 150 °C | 14.87 / 9.51 | 15.30 / 5.31 | 14.73 / 5.62 | 1.81 / 1.33 |

The current shift and the PM-flux offset do not reduce the $E_Q$ error at all. The shift slightly increases it. They halve the $E_m$ error. The fixed mode reduces both by a factor of 7–15.

### 4.4 Q4: dependence on the fitting method

All residuals differ by at most 1 point between TPS and measured co-energy in Table 1, and by at most 2.3 points in Table 3. The fitted coefficients differ by less than 0.6 A, or 0.013 in mode coefficient. The conclusions do not depend on the fitting method.

### 4.5 Post-hoc diagnostic: uniform flux scaling

This check was added after Fig. 2 was seen. It tests whether the change behaves like a uniform scaling of the flux.

| Target | Scaling only, $(1+s)\boldsymbol\psi_{T_r}$ | Scaling + current shift | $s$ (scaling + shift) | $\Delta I$ (scaling + shift) |
|---|---|---|---|---|
| −40 °C | 50.6 % (50.2) | 22.5 % (23.2) | +6.7 % | +32.7 A |
| 25 °C | 45.6 % (44.8) | 21.1 % (22.1) | +3.3 % | +14.4 A |
| 150 °C | 40.5 % (40.3) | 20.5 % (20.1) | −5.3 % | −20.9 A |

(Measured co-energy; TPS in parentheses. Linear interpolation for the shift.) Adding a scaling term halves the residual of the shift, but the fit remains clearly worse than the fixed mode.

## 5. Discussion

**Q1.** None of the magnet-only patterns represents the rossia temperature change. The current shift and the PM-flux offset leave 78–90 %, five to twelve times the fixed mode (7–14 %). The interpolation floor (≤0.17 %) and the fitting method (Q4) do not explain this. The dominant missing part is a q-axis change of the same size as the d-axis change, nearly uniform in the saturated region, and opposite in sign to the shift prediction.

This agrees with the premise the shift model violates. In the rossia data the stator core is held at the magnet temperature, and the rossia core material has temperature-dependent magnetic properties. The current-shift derivation requires a temperature-independent core reluctance. A core whose saturation level changes with temperature mainly affects the q-axis path, which carries little magnet flux. This interpretation fits the uniform cold-increase / hot-decrease of $\psi_q$ and the partial success of the scaling term (§4.5). It is an interpretation, not a separation: in this data set the two effects always change together, so their individual contributions cannot be identified.

The contrast with bio is informative. In bio, $\psi_q$ increased when hot, which is consistent with weaker magnets relieving saturation, and the shift reached 25 %. In rossia, $\psi_q$ decreases when hot, and the magnet-related relief is outweighed by the core effect.

**Q2.** The shift itself is well behaved. $\Delta I$ is linear in temperature to within 0.5 A, and its temperature coefficient (about −0.16 %/K) is plausible for the magnet. The magnet-strength component of the change is therefore captured consistently, even though the model as a whole fails. The fixed-mode coefficient, by contrast, is not linear in temperature, so a fixed mode calibrated at one temperature degrades by 3.6–6.6 points at an unused temperature. This favours a temperature model with curvature, such as the quadratic model of Report 3, or one built from separable mechanisms.

**Q3.** For the estimator, the magnet-only patterns fail exactly where the method relies on them. $E_Q$ is the current-direction projection used by the Q-based observer. Most of its temperature dependence comes from $\Delta\psi_q$, so a model without the core effect cannot track it. The fixed mode keeps the projected errors at 0.6–2.3 J, compared with 9–19 J without correction.

**Progress.** For rossia, the magnet-only single-scalar description is excluded. The fixed mode remains the reference model, with a known penalty for linear temperature scaling. The magnet contribution behaves linearly, which supports building a two-mechanism model (magnet + core) once the two effects can be observed separately.

### Limitations

- The data come from one motor model in FEA with a uniform motor temperature, so this is FEA-only evidence. Magnet and core effects are confounded by construction.
- The 75 °C map has the fewest identified points (89). Its TPS ψd CV (7.4 mWb) reflects the gaps near i_d = +721 A. The 150 °C measured co-energy surface has 55 Hessian-nonpositive nodes.
- The evaluation region includes positive $i_d$, unlike bio. The bio and rossia percentages are comparable in definition but not in region.
- §4.5 and the core interpretation were added after the results were seen.

## 6. Conclusions

**Q1.** On rossia, the current shift (86–90 %) and the PM-flux offset (78–85 %) do not represent the temperature change. The fixed mode leaves 7–14 %. The main unexplained part is a nearly uniform q-axis change, consistent with the temperature dependence of the rossia core material.

**Q2.** The shift is linear in temperature ($k\approx$ −0.33 A/K; prediction error below 0.5 A). The fixed-mode coefficient is not, which adds 3.6–6.6 points at an unused temperature.

**Q3.** The magnet-only patterns do not reduce the $E_Q$ error; the fixed mode reduces $E_Q$ and $E_m$ errors by a factor of 7–15.

**Q4.** TPS and measured co-energy give the same conclusions.

Decision: for rossia, shelve magnet-only single-scalar models and keep the fixed mode as the reference. Treat the temperature dependence as two mechanisms (magnet and core) in further work.

## 7. Outlook

**Achieved.** The three patterns were evaluated on four temperatures and two map fits. Their failure was traced to a q-axis change that the magnet-only premise cannot produce, and the linear behaviour of the magnet contribution was confirmed.

**Open issues.**

1. **Separating the mechanisms.** In JMAG, magnet and core temperatures can be set independently. Minimal cases are: magnet at −40 / 150 °C with the core at 75 °C, and the core at −40 / 150 °C with the magnet at 75 °C. They would show whether the magnet effect alone follows the current shift, what shape the core effect has, and whether the two add.
2. **Estimation without an external stator-temperature input.** In operation, core and magnet temperatures differ. With modes $\boldsymbol b_m$ and $\boldsymbol b_c$, $\Delta\boldsymbol\psi\approx\boldsymbol b_m\theta_m+\boldsymbol b_c\theta_c$. Q at a single operating point gives one equation for two unknowns. Using Q at several operating points, with slowly varying temperatures, can make the problem full-rank if the ratio $h_m/h_c$ ($h_x=\boldsymbol i^\mathsf T\boldsymbol b_x$) varies enough across operating points. The present results suggest that it does: the core effect concentrates in the saturated region. A second question is whether separation is needed at all. It is not needed if $g_m/h_m=g_c/h_c$ ($g_x=\boldsymbol t^\mathsf T\boldsymbol b_x$) at the operating points used for the P′ correction. Both questions can be evaluated statically once the separated JMAG cases exist. A preliminary estimate is possible now, using the scaling + shift decomposition of §4.5 as a proxy.
3. **Temperature dependence of the mode coefficient.** The fixed-mode coefficient is not linear in temperature. A model with curvature should be compared at an unused temperature, which the four rossia temperatures allow.

## Appendix A. Conventions

Power-invariant dq; currents in A; flux in Wb (tables and figures in mWb); pole pairs 4; temperature labels are the uniform motor temperature of each JMAG case. $E_Q$ and $E_m$ are in J (Wb·A); the conversion to P′ is $\delta P'=\omega_e\,\delta E_m$.

## Appendix B. Reproduction

1. Place the four VI files in `calibration_tools/jp_model/runcase/rossia_vi_cleaning/raw/` and run `clean_measured_vi.py`.
2. Run `run_processing_CLI.py --method measured_coenergy` and then `--method tps` for each `runcase/rossia_<T>/processing`.
3. Run `src/r3_rossia_current_shift.py` from the feature venv. Numerical results go to `simulation/results/current_shift/rossia/summary.json` (outside Git) and are copied to `report/figures/qw2p3_rossia_v5/summary.json`.

Runcases and generated maps are outside Git.

## Appendix C. References

1. K. Srinivasan, H. Hofmann, J. Sun, "Nonlinear Magnetics Model for Permanent Magnet Synchronous Machines Capturing Saturation and Temperature Effects," IEEE Trans. Energy Conversion, vol. 41, no. 1, pp. 311–324, 2026 (arXiv:2410.16240v3).
2. [Report 3 v4: current-shift model on bio](qw2p3_report_v4.md)
3. [Literature survey of principal challenges in IPM flux estimation](../design/ipm_flux_estimation_challenges_v1.md)
