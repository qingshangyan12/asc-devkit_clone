import json
import pathlib
import sys


UTIL_DIR = pathlib.Path(__file__).parents[2] / "cmake" / "asc" / "fwk_modules" / "util"
sys.path.insert(0, str(UTIL_DIR))

from ascendc_bin_param_build import BinParamBuilder


def test_relocatable_input_enables_aclgraph_super_kernel_combine(tmp_path):
    input_path = tmp_path / "add_custom.json"
    input_path.write_text(
        json.dumps({"op_list": [{"bin_filename": "add_custom_kernel"}]}),
        encoding="utf-8",
    )
    builder = BinParamBuilder("AddCustom")
    builder.set_op_super_config(
        {
            "--op_relocatable_kernel_binary=true",
            "--op_super_kernel_options=early-start=disable",
        }
    )

    generated_path = pathlib.Path(
        builder.gen_input_json_with_super_config(str(input_path))
    )

    generated = json.loads(generated_path.read_text(encoding="utf-8"))
    op_node = generated["op_list"][0]
    assert op_node["bin_filename"] == "add_custom_kernel_relocatable"
    assert op_node["extra_settings"] == [
        {
            "key": "super_kernel_sub_info",
            "value": {
                "enable_super_kernel": True,
                "super_kernel_options": "early-start=disable",
                "super_kernel_sub_loc": "middle",
            },
            "type": "addition",
        },
        {
            "key": "super_kernel_sub_combine",
            "value": True,
            "type": "addition",
        },
    ]


def test_generated_relocatable_node_uses_the_same_super_kernel_context():
    builder = BinParamBuilder("AddCustom")
    builder.set_op_super_config({"--op_relocatable_kernel_binary=true"})
    op_node = {"bin_filename": "add_custom_kernel_relocatable"}

    builder._apply_super_kernel_context(op_node)

    assert op_node["extra_settings"][0]["key"] == "super_kernel_sub_info"
    assert op_node["extra_settings"][0]["value"]["enable_super_kernel"] is True
    assert op_node["extra_settings"][1] == {
        "key": "super_kernel_sub_combine",
        "value": True,
        "type": "addition",
    }
