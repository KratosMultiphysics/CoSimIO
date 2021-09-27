# Tutorial for exchanging meshes with Kratos

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Mesh Exchange with Kratos](#mesh-exchange-with-kratos)
- [Next steps](#next-steps)

<!-- /code_chunk_output -->
---

This tutorial shows how to exchange meshes with Kratos. It is required to do [this tutorial](basic_data_exchange_with_kratos.md) first.

## Mesh Exchange with Kratos
Here we try to send the mesh (in the form of `CoSimIO::ModelPart`) to Kratos and get it back from it. Then we can check if both meshes are the same.
The Kratos side of this tutorial can be found in the [C++ tutorial](../cpp/mesh_exchange_with_kratos.md#mesh-exchange-with-kratos).

On the other side we use first export mesh and then import it back, following what was done in [this tutorial](integration_co_sim_io.md#mesh-exchange):

```c

```

This file can be found in [integration_tutorials/python/mesh_exchange.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/mesh_exchange.py)

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/bin/mesh_exchange_c_test & python3 path/to/import_export_mesh.py
```

## Next steps
In the [next tutorial](mapping.md), meshes are exchanged with Kratos and mapping is used.
