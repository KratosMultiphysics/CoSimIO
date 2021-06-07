# Documentation of the _CoSimIO_

This documentation contains explanations of the 


## Tutorials for integrating the _CoSimIO_
The tutorials can be found [here](tutorials/README.md)

### Some test
Hello

## Kratos CoSimulation
For using the _CoSimIO_ with Kratos it is necessary to build Kratos (separately). Basic explanations are given below with links for more detailed descriptions.

## Kratos CoSimulation Library Overview
Conceptually the Kratos CoSimulation consists of 3 main components:
1. **Data Transfer**: Any co-simulation process needs several data communications between different solvers/executables and Kratos. This includes raw data (in the form of vectors), meshes, and mesh data (like nodal and elemental data) or control signals. So a flexible data transfer mechanism one of the important components of the library.
2. **Coupling Solutions**: Coupling algorithms have many common parts like the strategy, convergence criteria, convergence accelerators, and predictors. To be seamlessly usable in Kratos such an algorithm should provide the same interface as internal solutions of Kratos. The Kratos CoSimulation provides many of these tools and algorithms generic enough to be used in different contexts.
3. **Mapping Between Meshes**: In many coupling problems the mesh in each domain is done considering the special requirements of that domain (like fluid, structure, etc.). This means that in those cases the meshes are not conformant between does domains and we should map our data from one to the other. Depending on the type of the coupling this may happen over some surfaces or volumes of the model. Kratos CoSimulation also provides this capability.

Based on these concepts these are the main components of the library:
1. **CoSimIO**: This library is in charge of the data transfer between different programs using inter process communication.
2. **CoSimulationApplication**: This is an application of Kratos which is located in [kratos/applications/CoSimulationApplication](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication) folder in the Kratos repository. This application provides a generic coupled solver with several convergence criteria, convergence accelerators, and predictors. Check the link for a more detailed overview.
3. **MappingApplication**: The standard mapping application of the Kratos located in [kratos/applications/MappingApplication](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/MappingApplication) folder. This application provides the mapping of data from one mesh to another one. It supports the 1D, 2D and 3D mapping with different types of mappings (like nearest node, nearest element, etc.). Check the link for a more detailed overview.

Kratos provides an extensive Python interface to its CoSimulation library which is used in the tutorials.


## Building Kratos with CoSimulation
Before starting the connection to Kratos we should build the Kratos with the necessary applications. The easiest way to build the Kratos would be using the standard_configure files provided in the [scripts](https://github.com/KratosMultiphysics/Kratos/tree/master/scripts) folder. These scripts would provide the Kratos core library but not the required CoSimulatation application and (optional but useful) Mapping application. For adding them you may copy for instance the standard_configure.sh to cosimulation_configure.sh and adding these two applications (you may keep or remove the rest):

```bash
# Set applications to compile
export KRATOS_APPLICATIONS=
add_app ${KRATOS_APP_DIR}/CoSimulationApplication
add_app ${KRATOS_APP_DIR}/MappingApplication
```

Now for running the Kratos, first you should add configure the python environment variables pointing to Kratos folder:
* `PYTHONPATH` should point to the Kratos binaries root (`path/to/kratos`) where you can find KratosMultiphysics sub folder
* `LD_LIBRARY_PATH` should point to the libs folder (`path/to/kratos/libs`) where you can find compiled libraries.

For more information about the Kratos build requirements, options please check the [Kratos install guide](https://github.com/KratosMultiphysics/Kratos/blob/master/INSTALL.md)





<!--


# Testing
[cpp](cpp/README.md)




## Welcome to GitHub Pages

You can use the [editor on GitHub](https://github.com/KratosMultiphysics/CoSimIO/edit/docs-improvement/docs/index.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/KratosMultiphysics/CoSimIO/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out. -->
