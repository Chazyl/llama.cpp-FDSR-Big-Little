# llama.cpp-FDSR-Big-Little
**Feature Driven Speculative Routing for Mixture of Experts (MoE)**

## The Vision: Breaking the VRAM Wall
We are currently facing an absurdity in the AI hardware landscape. VRAM is treated as the ultimate bottleneck, forcing users into cloud dependency and datacenters into financial black holes. 

This project, **FDSR-Big-Little**, introduces a new paradigm to MoE (Mixture of Experts) inference. By marrying **Feature Steering** (Semantic Analysis) with an ARM-inspired **Big.LITTLE** hardware architecture, we stop brute-forcing memory bandwidth and start routing weights intelligently.

This architecture was built to solve two massive industry problems:

### 1. The Pro-Laptop Dilemma (Sovereignty & Privacy)
There are millions of high-end consumer machines and developer laptops out there. Most feature a dedicated GPU (dGPU) with limited VRAM (6-8GB) and an integrated GPU (iGPU) with access to massive system RAM (32-64GB). 
Currently, running a serious MoE model locally means offloading to DDR system RAM, which kills performance. FDSR gives these machines the ability to run heavy, serious local MoE models natively, privately, and without cloud costs, by turning the iGPU into an active, compute-in-place secondary engine.

### 2. Datacenter VRAM Waste (Disaggregated Inference)
In datacenters, loading *every single expert* of a massive MoE model into $40k HBM3e VRAM is incredibly inefficient. Some experts are "Cold" (used for <2% of tokens). FDSR allows for the mutualization of cold experts across cheaper, slower hardware pools, reducing overall VRAM consumption and slashing operational costs, without a latency penalty.

---

## ⚙️ How it Works: The FDSR Paradigm

Standard `llama.cpp` waits for the Gating Network (the Router) to decide which expert is needed at the very last microsecond. If that expert is in system RAM, the pipeline stalls.

**Feature Driven Speculative Routing (FDSR)** flips this logic. 
Because the MoE router uses activations to pick experts, and because we can map these activations using Feature Steering vectors, **we can predict the destination before the layer is even computed.**

We use a highly optimized, lightweight **SQLite database** to statistically profile and categorize experts into "Hot", "Warm", and "Cold" based on semantic context. 

### The Tiered Memory Architecture (Cache L1/L2/L3)

Instead of a flat memory pool, FDSR maps the model onto a tiered hardware topology:

*  **Cache L1 (The "Big" Node - Primary dGPU):**
    *   *Hardware:* Fast PCIe 4.0/5.0 dGPU (e.g., RTX 4090, RX 6750 XT, or Datacenter H100).
    *   *Workload:* Hosts the KV Cache, Self-Attention layers, the Gating Network, and the Top % of "Hot" experts.
*   🌡️ **Cache L2 (The "Little" Node - Secondary GPU / iGPU):**
    *   *Hardware:* Slower PCIe links, older dGPUs (e.g., RX 5600 XT), or iGPUs (e.g., Radeon 780M).
    *   *Workload:* Hosts the "Warm" experts. **Compute-in-place.** Instead of moving heavy weights across the PCIe bus, we send the lightweight activation to the L2 node, compute it there, and return the result.
*   ❄️ **Cache L3 (The Graveyard - System RAM / Disaggregated Pool):**
    *   *Hardware:* System DDR4/DDR5.
    *   *Workload:* Stores the "Cold" experts. 

### ⚡ Lookahead Asynchronous Prefetching
When the SQLite semantic router detects a specific concept vector in the early layers (e.g., `[Concept_Python_Syntax]`), it queries the database: *"Which cold experts correlate with this vector in deeper layers?"*
FDSR then triggers an **asynchronous DMA prefetch** of those specific experts from Cache L3 to Cache L1 *while* the primary GPU is still computing the earlier layers. By the time the MoE router asks for the expert, it's already in the fast VRAM. Zero stuttering. Zero latency hit.

---

## 🛠️ The Semantic Routing Engine (SQLite)

The core logic bypasses the need for heavy neural predictors. It relies on a deterministic SQLite routing table bridging the gap between representation engineering and memory allocation.

```sql
-- Abstract representation of the FDSR Semantic Table
SELECT expert_id, layer_id, target_hardware 
FROM fdsr_expert_profiling 
WHERE semantic_vector_id = 'vector_logic_math' 
AND temperature_status = 'COLD';
-- Action: Trigger Asynchronous Prefetch from L3 to L1
```

## 🎯 Use Cases & Hardware Topologies
FDSR is highly flexible and thrives on heterogeneous hardware setups:
*   **The Dev Laptop:** dGPU (RTX 3060 Mobile) + iGPU (Intel Iris Xe / AMD 780M).
*   **The "Frankenstein" Rig:** Combining mismatched architectures (e.g., RDNA2 + RDNA1 via PCIe Passthrough / Proxmox) to achieve 18GB+ of effective VRAM routing.
*   **Enterprise Datacenter:** H100 (Hot Nodes) + High-RAM CPU Servers (Cold Pools).
