#include "Halide.h"

using namespace Halide;

class DmaPipeline : public Generator<DmaPipeline> {
public:
    Input<Buffer<uint8_t>> input{"input", 4};
    Output<Buffer<uint8_t>> output{"output", 4};

    void generate() {
        Var x{"x"}, y{"y"}, c("c"), t("t");

        // We need a wrapper for the output so we can schedule the
        // multiply update in tiles.
        Func copy("copy");

        copy(x, y, c, t) = input(x, y, c, t);

        output(x, y, c, t) = copy(x, y, c, t) * 2;

        Var tx("tx"), ty("ty");

        // Break the output into tiles.
        const int tile_width = 256;
        const int tile_height = 128;

        output
            .compute_root()
            .tile(x, y, tx, ty, x, y, tile_width, tile_height, TailStrategy::RoundUp);

        // Schedule the copy to be computed at tiles with a
        // circular buffer of two tiles.
        copy
            .compute_at(output, tx)
            .store_at(output, tx)
            .fold_storage(x, tile_width * 2)
            .copy_to_host();
    }

};

HALIDE_REGISTER_GENERATOR(DmaPipeline, dma_pipeline_err)