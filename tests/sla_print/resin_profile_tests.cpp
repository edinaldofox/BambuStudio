#include <catch2/catch.hpp>

#include <libslic3r/PresetBundle.hpp>

#include <iterator>

namespace {

constexpr const char *printer_name = "Generic SL1-compatible MSLA 6.08in (Calibration)";
constexpr const char *print_name = "0.05mm Standard @Generic SL1 MSLA";
constexpr const char *material_name = "Generic Standard Resin @Generic SL1 MSLA";

TEST_CASE("SLA preset type names round-trip across local and cloud storage", "[SLA][Preset]")
{
    REQUIRE(Slic3r::Preset::get_iot_type_string(Slic3r::Preset::TYPE_SLA_PRINT) == "sla_process");
    REQUIRE(Slic3r::Preset::get_iot_type_string(Slic3r::Preset::TYPE_SLA_MATERIAL) == "sla_material");
    REQUIRE(Slic3r::Preset::get_type_from_string("sla_print") == Slic3r::Preset::TYPE_SLA_PRINT);
    REQUIRE(Slic3r::Preset::get_type_from_string("sla_process") == Slic3r::Preset::TYPE_SLA_PRINT);
    REQUIRE(Slic3r::Preset::get_type_from_string("sla_materials") == Slic3r::Preset::TYPE_SLA_MATERIAL);
    REQUIRE(Slic3r::Preset::get_type_from_string("sla_material") == Slic3r::Preset::TYPE_SLA_MATERIAL);
}

TEST_CASE("GenericResin bundle loads as an SLA configuration", "[SLA][PresetBundle]")
{
    Slic3r::PresetBundle bundle;
    REQUIRE(bundle.has_defauls_only());
    const size_t loaded = bundle.load_vendor_configs_from_json(
        RESIN_PROFILE_DIR, "GenericResin", Slic3r::PresetBundle::LoadSystem,
        Slic3r::ForwardCompatibilitySubstitutionRule::Disable).second;

    REQUIRE(loaded >= 3);
    REQUIRE(std::distance(bundle.sla_prints.begin(), bundle.sla_prints.end()) == 1);
    REQUIRE(std::distance(bundle.sla_materials.begin(), bundle.sla_materials.end()) == 1);
    REQUIRE(std::distance(bundle.printers.begin(), bundle.printers.end()) == 1);
    REQUIRE_FALSE(bundle.has_defauls_only());

    REQUIRE(bundle.printers.select_preset_by_name(printer_name, true));
    REQUIRE(bundle.sla_prints.select_preset_by_name(print_name, true));
    REQUIRE(bundle.sla_materials.select_preset_by_name(material_name, true));

    const Slic3r::DynamicPrintConfig config = bundle.full_config();
    REQUIRE(Slic3r::Preset::printer_technology(config) == Slic3r::ptSLA);
    REQUIRE(config.opt_string("sla_print_settings_id") == print_name);
    REQUIRE(config.opt_string("sla_material_settings_id") == material_name);
    REQUIRE(config.opt_float("display_width") == Approx(133.0));
    REQUIRE(config.opt_float("display_height") == Approx(75.0));

    Slic3r::AppConfig app_config;
    bundle.export_selections(app_config);
    REQUIRE(app_config.get("presets", "sla_print") == print_name);
    REQUIRE(app_config.get("presets", "sla_material") == material_name);
}

TEST_CASE("Filament-only loading does not import SLA presets", "[SLA][PresetBundle]")
{
    Slic3r::PresetBundle bundle;
    const size_t loaded = bundle.load_vendor_configs_from_json(
        RESIN_PROFILE_DIR, "GenericResin",
        Slic3r::PresetBundle::LoadSystem | Slic3r::PresetBundle::LoadFilamentOnly,
        Slic3r::ForwardCompatibilitySubstitutionRule::Disable).second;

    REQUIRE(loaded == 0);
    REQUIRE(bundle.sla_prints.begin() == bundle.sla_prints.end());
    REQUIRE(bundle.sla_materials.begin() == bundle.sla_materials.end());
}

} // namespace
