# Tutorial for connecting to Kratos and performing basic data exchange

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Connecting to Kratos](#connecting-to-kratos)
- [Data Exchange with Kratos](#data-exchange-with-kratos)
- [Next steps](#next-steps)

<!-- /code_chunk_output -->
---

This tutorial shows how to establish a connection to Kratos and to do basics data exchange with Kratos. It is required to do [this tutorial](integration_co_sim_io.md) first.

Furthermore it is required to compile Kratos which is described [here](../../kratos_build.md).

## Connecting to Kratos
Instead of connecting two instances of the same code as shown in the [previous tutorial](integration_co_sim_io.md#connecting-and-disconnecting), here one instance is connected to Kratos.
For this we use the same code from the previous tutorial.

The Kratos side of this tutorial can be found in the [C++ tutorial](../cpp/basic_data_exchange_with_kratos.md#connecting-to-kratos).

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/connect_disconnect_c & python3 path/to/connect_disconnect.py
```

## Data Exchange with Kratos
Here we try to send some data to Kratos and get it back from it. Then we can check if both data are the same.

The Kratos side of this tutorial can be found in the [C++ tutorial](../cpp/basic_data_exchange_with_kratos.md#data-exchange-with-kratos).

On the other side, we use first export data and then import it back, following what was done in [this tutorial](integration_co_sim_io.md#data-exchange):

```c

```

This file can be found in [integration_tutorials/python/data_exchange.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/data_exchange.py)

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/bin/data_exchange_c_test & python3 path/to/import_export_data.py
```

## Next steps
In the [next tutorial](mesh_exchange_with_kratos.md), meshes are exchanged with Kratos
