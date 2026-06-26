# Roadmap

This document lays out the planned development path for Shrodyne, from the first build-system commit to the 1.0 release. Each milestone builds on the last, moving from raw rendering infrastructure toward an increasingly accurate and complete chemistry-focused atomic simulation.

Shrodyne is being developed as two related but separate simulations:

- **Big Sim** — the primary focus, and the subject of this roadmap. A multi-atom, chemistry- and materials-oriented simulation: electrons are treated as individual quantum wavefunctions in an effective potential (a Hartree-style approximation), nuclei are treated as bound collections of protons and neutrons — not individual quarks/gluons — under a phenomenological strong-force model with a stochastic correction term, and unstable isotopes decay via a fully deterministic weak-force treatment. This is the sim used to explore real chemical bonding, organic structures, and microstructures.
- **Small Sim** — a lower-priority, later-stage showcase: a single nucleus simulated down to its quark/gluon structure, optimized for visual and educational insight into quark confinement rather than for chemistry accuracy. Work on this begins only after the Big Sim reaches v1.0, and its own roadmap will be written once that starts.

---

## v0.1 — Foundations

**Goal:** Get a working build pipeline and a minimal rendering loop running, with basic camera and time controls in place.

- Set up the CMake build system so the project can be configured and compiled cleanly across platforms.
- Establish the core Vulkan rendering pipeline: swapchain, basic shaders, and a render loop capable of drawing simple test geometry (placeholder shapes standing in for future particles).
- Implement camera movement and zoom, so the user can navigate the 3D scene freely.
- Add a simulation time control system: the ability to speed up, slow down, or pause the flow of simulated time, independent of rendering frame rate.

This version contains no real physics yet — it's the scaffolding everything else will be built on.

---

## v0.2 — The Nucleus

**Goal:** Simulate an atomic nucleus as a bound system of protons and neutrons.

- Model protons and neutrons as individual particles, not decomposed into quarks/gluons — that level of detail is reserved for the future Small Sim.
- Implement a phenomenological strong-force potential (e.g. a Yukawa or Woods-Saxon style model) that binds nucleons together at short range, balanced against Coulomb repulsion between protons.
- Layer a stochastic noise term on top of this mean-field potential to approximate the quantum fluctuations a purely static model would otherwise miss, keeping the nucleus from behaving as an artificially rigid object.
- Allow multiple protons and neutrons to be grouped into nuclei, opening the door to simulating different elements and isotopes.

This version is purely about nuclear structure: how nucleons settle into a stable arrangement, without yet touching on how nuclei change or decay over time.

---

## v0.3 — Electrons and Atomic Structure

**Goal:** Complete the atom with a genuinely quantum electron treatment.

- Add electrons as individual wavefunctions evolving under the Schrödinger equation, each moving in an effective potential that combines the nucleus's field with the averaged repulsion from the other electrons present.
- Represent electron position as a true probability density (electron cloud), not a fixed point or classical orbit.
- Apply the Pauli exclusion principle to govern how electrons fill available states.
- Support energy level transitions, including photon absorption and emission as electrons move between orbitals.

With this milestone, Shrodyne can represent a complete, quantum-grounded atom — nucleus and electron cloud together.

---

## v0.4 — Radioactivity and the Weak Force

**Goal:** Let unstable isotopes decay according to real weak-interaction dynamics, not random rolls.

- Implement deterministic decay-amplitude evolution (a Wigner-Weisskopf style treatment): the parent nucleus's quantum amplitude decreases continuously over time while the daughter products' amplitude grows correspondingly, integrated as a differential equation rather than sampled stochastically.
- Derive decay constants from the underlying interaction Hamiltonian (Fermi's golden rule) rather than hardcoding half-life lookup tables.
- Tie decay behavior to the binding-energy result produced by the v0.2 nuclear model, so stability and instability emerge from the simulation instead of being asserted.
- Optionally support a discrete "decay event" visualization layer for rendering purposes (a single sampled pop/flash), without that sampling affecting the underlying deterministic physics.

---

## v0.5 — Multi-Atom Chemistry

**Goal:** Move from isolated atoms to interacting systems — real bonding.

- Support multiple atoms simultaneously, with electron clouds that interact across atoms.
- Allow emergent bond formation (covalent, ionic, etc.) to arise naturally from the underlying electron and electromagnetic force calculations, rather than being scripted or assumed.
- Lay the groundwork for the planned material property calculation library — the long-term goal of using the simulation to search for and predict organic compounds and microstructures.

---

## v0.6 — Interface and Usability Pass

**Goal:** Make the simulation approachable for people who aren't already physics experts.

- Redesign and polish the UI: clearer menus, labeled controls, and in-context explanations of what's being shown on screen.
- Add toggles for different visualization modes (e.g., switching between a "particle" view and a "probability cloud" view).
- Improve control responsiveness (camera, time scaling, selection of particles/elements to simulate).
- General performance optimization to keep the simulation smooth as scene complexity grows.

---

## v1.0 — First Stable Release (Big Sim)

**Goal:** Ship a polished, reliable first release of the Big Sim.

- Comprehensive bugfixing pass across rendering, simulation logic, and UI.
- Double-check physical accuracy and visual correctness across nucleon binding, electron orbitals, bonding behavior, and decay dynamics.
- Verify stability across different hardware/configurations supported by the Vulkan backend.
- Finalize documentation so new users and contributors can get started without confusion.

---

## Future — Small Sim (Post-1.0)

**Goal:** A standalone, single-nucleus simulation built down to quark/gluon structure, prioritizing visual and educational impact over chemistry-level accuracy.

- Model a proton as a bound system of three valence quarks (two up, one down), with gluons depicted as the carriers of the strong force, constantly exchanged between quarks (color charge exchange).
- Provide a visually faithful depiction of quark confinement, illustrating why quarks are never observed in isolation.
- Real-time lattice-QCD-style calculation is computationally extreme even on dedicated supercomputers, so feasibility and scale (toy lattice size, number of nucleons) will be evaluated once work begins, with a fallback to an effective nucleon-level model if real-time performance isn't achievable.
- Detailed milestones for this track will be added once Big Sim v1.0 ships.

---

*This roadmap is a living document and may shift as development progresses and priorities change.*
