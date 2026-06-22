# Roadmap

This document lays out the planned development path for Shrodyne, from the first build-system commit to the 1.0 release. Each milestone builds on the last, moving from raw rendering infrastructure toward an increasingly accurate and complete particle/atomic simulation.

---

## v0.1 — Foundations

**Goal:** Get a working build pipeline and a minimal rendering loop running, with basic camera and time controls in place.

- Set up the CMake build system so the project can be configured and compiled cleanly across platforms.
- Establish the core Vulkan rendering pipeline: swapchain, basic shaders, and a render loop capable of drawing simple test geometry (placeholder shapes standing in for future particles).
- Implement camera movement and zoom, so the user can navigate the 3D scene freely.
- Add a simulation time control system: the ability to speed up, slow down, or pause the flow of simulated time, independent of rendering frame rate.

This version contains no real physics yet — it's the scaffolding everything else will be built on.

---

## v0.2 — The Proton

**Goal:** Simulate a single proton, built from its constituent quarks and gluons.

- Model a proton as a bound system of three valence quarks (two up quarks and one down quark).
- Represent gluons as the carriers of the strong force, visually showing how they are constantly exchanged between quarks to hold the proton together (color charge exchange).
- Provide a simplified but visually faithful depiction of quark confinement — illustrating why quarks are never observed in isolation.
- This is the first version where the simulation actually demonstrates a real physical structure rather than placeholder shapes.

---

## v0.3 — Neutrons and Nuclei

**Goal:** Extend the simulation beyond a single proton to full atomic nuclei.

- Add the neutron (one up quark, two down quarks) as a second baryon type, alongside the proton.
- Implement the residual strong force (nuclear force) — the short-range attraction between protons and neutrons that arises as a "leftover" of the strong force binding quarks inside each nucleon, even though each nucleon is individually color-neutral.
- Allow multiple protons and neutrons to be grouped into stable nuclei, opening the door to simulating different elements and isotopes.
- This version is purely about structure: how nucleons stack together into a nucleus, without yet touching on how nuclei can change or decay over time.

---

## v0.4 — Electrons and Atomic Structure

**Goal:** Complete the atom by adding electrons and the quantum rules that govern them.

- Add electrons as particles bound to a nucleus by the electromagnetic force.
- Implement electron shells/orbitals and represent electron position as a probability distribution (electron cloud) rather than a fixed point, staying true to quantum mechanics.
- Apply the Pauli exclusion principle to govern how electrons fill available states.
- Support energy level transitions, including photon absorption and emission as electrons move between orbitals.
- With this milestone, Shrodyne can represent a complete, simplified atom — nucleus and electron cloud together.

---

## v0.5 — Interface and Usability Pass

**Goal:** Make the simulation approachable for people who aren't already physics experts.

- Redesign and polish the UI: clearer menus, labeled controls, and in-context explanations of what's being shown on screen.
- Add toggles for different visualization modes (e.g., switching between a "particle" view and a "probability cloud" view).
- Improve control responsiveness (camera, time scaling, selection of particles/elements to simulate).
- General performance optimization to keep the simulation smooth as scene complexity grows.

---

## v1.0 — First Stable Release

**Goal:** Ship a polished, reliable first release.

- Comprehensive bugfixing pass across rendering, simulation logic, and UI.
- Double-check physical accuracy and visual correctness across every particle type and interaction added so far (quarks, gluons, protons, neutrons, nuclear binding, electrons).
- Verify stability across different hardware/configurations supported by the Vulkan backend.
- Finalize documentation so new users and contributors can get started without confusion.

---

*This roadmap is a living document and may shift as development progresses and priorities change.*
