{
  description = "Tablog";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };

      version = "0.0.1";

      commonDeps = with pkgs; [
        cmake
        gcc
        gnumake
      ];

      mkTTP2Package =
        {
          pname,
          buildTarget,
          enableLib ? false,
          enableClient ? false,
          extraInputs ? [ ],
        }:
        pkgs.stdenv.mkDerivation {
          inherit pname version;
          src = ./.;

          buildInputs = commonDeps ++ extraInputs;

          configurePhase = ''
            cmake -B build -S $src \
              -DCMAKE_BUILD_TYPE=Release \
              -DDEF_TTP2=${if enableLib then "ON" else "OFF"} \
              -DDEF_CLIENT=${if enableClient then "ON" else "OFF"} \
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

    in
    {
      packages.${system} =
        let
          lib = mkTTP2Package {
            pname = "libtablog";
            buildTarget = "tablog";
            enableLib = true;
          };
        in
        {
          inherit lib;

          client = mkTTP2Package {
            pname = "tablog-client";
            buildTarget = "tablog-client";
            enableClient = true;
            extraInputs = [ lib ];
          };

          full = mkTTP2Package {
            pname = "libtablog-full";
            buildTarget = "all";
            enableLib = true;
            enableClient = true;
          };

          default = self.packages.${system}.lib;
        };

      devShells.${system}.default = pkgs.mkShell {
        packages = commonDeps ++ [
          pkgs.bridge-utils
          pkgs.clang-tools
        ];

        shellHook = ''
          git status
        '';
      };
    };
}
