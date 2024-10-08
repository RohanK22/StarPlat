## Build

The initial step of cloning the repository is the same across all operating systems. This guide, however, focuses specifically on transferring and editing files within the Aquacluster environment due to its unique requirements.

- **Clone Locally**: Use Git to clone the repository to your local machine.
   ```bash
   git clone https://github.com/ashwina/StarPlat
   ```


- **Transfer Files**: Use the `scp` command to securely transfer the project files from your local machine to the Aquacluster environment.
   ```bash
   scp -r /path/to/StarPlat/* rnintern@aqua.iitm.ac.in:~/scratch/username
   ```


#### Aquacluster

```
cd /path/to/StarPlat/src
make
```
Optional: If you're working on a project that requires a specific version of g++, or if you need to ensure that your build process uses the g++ compiler located at `/lfs/sware/gcc9.2.0/bin/g++`, you can set this path in the `CC` environment variable. This is particularly useful in build environments where multiple compiler versions are installed or the default compiler is unsuitable for your project.

#### Linux
This section is for Linux MPI

#### MacOS

By default, MacOS comes with the `clang` compiler, which can cause syntax errors when running the `make` command due to differences between `gcc` and `clang` semantics. To resolve this, follow these steps:

#### Installation:

1. Install `gcc` using Homebrew:

   ```sh
   brew install gcc
   ```

2. Check the installation location of the `g++` binaries:

   ```sh
   which brew
   ll /opt/homebrew/bin/g++-*
   ```

3. Create symbolic links to the installed version, (presumes that the above command returns gcc-13):

   ```sh
   sudo ln -sf /opt/homebrew/bin/gcc-13 /usr/local/bin/gcc
   sudo ln -sf /opt/homebrew/bin/g++-13 /usr/local/bin/g++
   sudo ln -sf /opt/homebrew/bin/c++-13 /usr/local/bin/c++
   sudo ln -sf /opt/homebrew/bin/cpp-13 /usr/local/bin/cpp
   ```

   Note: Modifying anything in `/usr/bin/*` is discouraged and may require disabling SIP (System Integrity Protection). Therefore, we create symlinks to `/usr/local/bin/*`.

4. After installation, check the versions:

   ```sh
   g++ --version # Should return clang version
   g++-13 --version # Should return (Homebrew GCC)
   ```

5. Update the Makefile (`StarPlat/src/Makefile`) to use the symlinked `g++`:

   ```make
   CC = /usr/local/bin/g++
   ```
6. Add `%token return_func` in the file `lrparser.y` (`StarPlat/src/parser/lrparser.y`)

7. Run make in the `src` directory:

   ```sh
   cd /path/to/StarPlat/src
   make
   ```