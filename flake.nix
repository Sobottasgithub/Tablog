{
  description = "Tablog";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
      forEachSystem = nixpkgs.lib.genAttrs supportedSystems;
    in
    {
      packages = forEachSystem (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
          version = "0.2.1";

          commonDeps = with pkgs; [
            cmake
            gcc
            gnumake
            tomlplusplus
          ];

          mkTablogPackage =
            {
              pname,
              buildTarget,
              enableLib ? false,
              enableTest ? false,
              extraInputs ? [ ],
            }:
            pkgs.stdenv.mkDerivation {
              inherit pname version;
              src = ./.;

              buildInputs = commonDeps ++ extraInputs;

              configurePhase = ''
                cmake -B build -S $src \
                  -DCMAKE_BUILD_TYPE=Release \
                  -DDEF_TABLOG=${if enableLib then "ON" else "OFF"} \
                  -DDEF_TEST=${if enableTest then "ON" else "OFF"} \
              '';

              buildPhase = ''
                cmake --build build \
                  --target ${buildTarget} \
                  -j$NIX_BUILD_CORES
              '';

              installPhase = ''
                cmake --install build --prefix=$out
                cp LICENSE $out/
              '';
            };

          lib = mkTablogPackage {
            pname = "libtablog";
            buildTarget = "tablog";
            enableLib = true;
          };
        in
        {
          inherit lib;

          test = mkTablogPackage {
            pname = "tablog-test";
            buildTarget = "tablog-test";
            enableTest = true;
            extraInputs = [ lib ];
          };

          full = mkTablogPackage {
            pname = "libtablog-full";
            buildTarget = "all";
            enableLib = true;
            enableTest = true;
          };

          default = lib;
        }
      );

      devShells = forEachSystem (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
          commonDeps = with pkgs; [
            cmake
            gcc
            gnumake
            tomlplusplus
          ];
        in
        {
          default = pkgs.mkShell {
            packages = commonDeps ++ [
              pkgs.bridge-utils
              pkgs.clang-tools
            ];

            shellHook = ''
              git status
            '';
          };
        }
      );
    };
}
