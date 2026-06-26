# Shrodyne

A quantum-grounded chemistry and physics simulation, built to be an accurate approximation of how atoms actually behave — not just a visual demo. Shrodyne visualizes how probability distributions flow through space, and how chemistry truly works under the hood.

This project is built with C++, CMake, and Vulkan, using GPU compute shaders for the underlying physics as well as rendering, so the simulation and visualization run from the same data without a CPU round-trip.

## Why?

This project grew out of my interest in quantum physics and chemistry. I wanted simulations like this to not just look nice, but also explain how everything actually works for others. Beyond that, I'm aiming to build an automatic material property calculation library, which I plan to use in a Minecraft mod I'm working on.

## Two Simulations

Shrodyne is split into two separate simulations, since chemistry-scale accuracy and nucleus-internals-scale accuracy pull in very different directions computationally:

- **Big Sim** (primary focus) — a multi-atom chemistry and materials simulation. Electrons are modeled as individual quantum wavefunctions in an effective potential; nuclei are modeled as bound protons and neutrons under a phenomenological strong-force potential with a stochastic correction term; weak-force decay is treated with fully deterministic amplitude evolution rather than random sampling. Built to support real exploration of bonding, organic structures, and microstructures.
- **Small Sim** (future, lower priority) — a single-nucleus showcase simulated down to quark/gluon structure, focused on visual and educational insight into quark confinement rather than chemistry-level accuracy. Development begins after the Big Sim reaches v1.0.

## Roadmap

See [ROADMAP.md](./ROADMAP.md) for a detailed breakdown of each milestone.

- **v0.1** — Build system, basic Vulkan rendering, camera and time controls.
- **v0.2** — The nucleus: protons and neutrons bound by a phenomenological strong-force model.
- **v0.3** — Electrons and atomic structure: quantum wavefunctions, orbitals, Pauli exclusion, energy transitions.
- **v0.4** — Radioactivity: deterministic weak-force decay dynamics.
- **v0.5** — Multi-atom chemistry: real bonding between atoms.
- **v0.6** — Interface enhancements for usability.
- **v1.0** — First release of the Big Sim, after bugfixes and a thorough accuracy/stability pass.
- **Future** — Small Sim: single-nucleus quark/gluon showcase.

## Changelog

See [CHANGELOG.md](./CHANGELOG.md) for a record of completed work and notable changes.

## Contact Me

The best way to reach me is on Discord — feel free to DM me!
@bread_mme
[![Discord](https://img.shields.io/badge/Discord-Join%20Us-5865F2?logo=discord&logoColor=white)](https://discord.gg/Wx9YwhcPyT)
