//$$FILE$$
//$$VERSION$$
//$$DATE$$
//$$LICENSE$$


/*!
** \file CifExcept.C
**
** \brief Implementation file for CifExcept class.
*/


#include "CifExcept.h"


using std::string;


bool CifExcept::CanBeUnknown(const string& itemName)
{
    // Optional, parent item, has value "?"
    if (itemName == "_atom_site.pdbx_PDB_ins_code")
        return (true);

    return (false);
}


bool CifExcept::CanBeInapplicable(const string& itemName)
{
    // Non-key, mandatory, parent; It seems that this item is always set to '.'
    if (itemName == "_atom_site.label_alt_id")
        return (true);
    // Non-key, mandatory, parent and child; It is '.' only if group_PDB is
    // set to HETATM, for other group_PDB it has some numeric value.
    else if (itemName == "_atom_site.label_seq_id")
        return (true);

    // Non-key, mandatory, child; It seems that this item is always set to '.'
    else if (itemName == "_struct_conn.ptnr1_label_seq_id")
        return (true);

    // Non-key, mandatory, child; It seems that this item is randomly set to '.'
    // depending on some conditions.
    else if (itemName == "_struct_conn.ptnr2_label_seq_id")
        return (true);

    // Non-key, mandatory, child; It seems that this item is always set to '.'
    else if (itemName == "_struct_site_gen.label_atom_id")
        return (true);

    // Non-key, mandatory, child; It seems that this item is randomly set to '.'
    // depending on some conditions.
    else if (itemName == "_struct_site_gen.label_seq_id")
        return (true);

    // Non-key, non-mandatory, child; It seems that this item is always set
    // to '.'
    else if (itemName == "_atom_site_anisotrop.pdbx_label_alt_id")
        return (true);

    // Non-key, non-mandatory, non-child, non-parent; It seems that this item
    // is randomly set to '.',  depending on some conditions.
    else if (itemName == "_chem_comp.mon_nstd_flag")
        return (true);

    // Non-key, non-mandatory, non-child, non-parent; It seems that this item
    // is always set to '.'
    else if (itemName == "_pdbx_database_status.SG_entry")
        return (true);

    // Non-key, non-mandatory, non-child, non-parent; It seems that this item
    // is always set to '.'
    else if (itemName == "_pdbx_nonpoly_scheme.pdb_ins_code")
        return (true);

    // Non-key, non-mandatory, parent; It seems that this item
    // is always set to '.'
    else if (itemName == "_pdbx_poly_seq_scheme.pdb_ins_code")
        return (true);

    // Non-key, non-mandatory, non-child, non-parent; It seems that this item
    // is always set to '.'
    else if (itemName == "_reflns.observed_criterion_sigma_I")
        return (true);

    // Non-key, non-mandatory, child; It seems that this item
    // is always set to '.'
    else if (itemName == "_struct_ref.biol_id")
        return (true);

    // Non-key, non-mandatory, child; It seems that this item
    // is always set to '.'
    else if (itemName == "_struct_site_gen.label_alt_id")
        return (true);

    return (false);
}


bool CifExcept::IsBadParentRelation(const string& itemName)
{
#ifdef BAD_RELATION
chem_comp_link                           1  '_chem_comp_link.type_comp_1'                         '_chem_comp.type'                           chem_comp
chem_comp_link                           2  '_chem_comp_link.type_comp_2'                         '_chem_comp.type'                           chem_comp
#endif

    if (itemName == "_chem_comp.type")
        return (true);

#ifdef BAD_RELATION
pdbx_struct_ref_seq_deletion             1  '_pdbx_struct_ref_seq_deletion.asym_id'               '_pdbx_poly_seq_scheme.asym_id'             pdbx_poly_seq_scheme
#endif

    else if (itemName == "_pdbx_poly_seq_scheme.asym_id")
        return (true);

#ifdef BAD_RELATION
pdbx_struct_sheet_hbond                  4  '_pdbx_struct_sheet_hbond.range_id_1'                 '_struct_sheet_range.id'                    struct_sheet_range
pdbx_struct_sheet_hbond                  5  '_pdbx_struct_sheet_hbond.range_id_2'                 '_struct_sheet_range.id'                    struct_sheet_range
struct_sheet_hbond                       6  '_struct_sheet_hbond.range_id_1'                      '_struct_sheet_range.id'                    struct_sheet_range
struct_sheet_hbond                       7  '_struct_sheet_hbond.range_id_2'                      '_struct_sheet_range.id'                    struct_sheet_range
struct_sheet_order                       2  '_struct_sheet_order.range_id_1'                      '_struct_sheet_range.id'                    struct_sheet_range
struct_sheet_order                       3  '_struct_sheet_order.range_id_2'                      '_struct_sheet_range.id'                    struct_sheet_range
struct_sheet_topology                    2  '_struct_sheet_topology.range_id_1'                   '_struct_sheet_range.id'                    struct_sheet_range
struct_sheet_topology                    3  '_struct_sheet_topology.range_id_2'                   '_struct_sheet_range.id'                    struct_sheet_range
#endif

    else if (itemName == "_struct_sheet_range.id")
        return (true);

#ifdef BAD_RELATION
atom_site                                5  '_atom_site.label_atom_id'                            '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_angle                          1  '_chem_comp_angle.atom_id_1'                          '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_angle                          2  '_chem_comp_angle.atom_id_2'                          '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_angle                          3  '_chem_comp_angle.atom_id_3'                          '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_bond                           1  '_chem_comp_bond.atom_id_1'                           '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_bond                           2  '_chem_comp_bond.atom_id_2'                           '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_chir                           2  '_chem_comp_chir.atom_id'                             '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_chir_atom                      2  '_chem_comp_chir_atom.atom_id'                        '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_plane_atom                     2  '_chem_comp_plane_atom.atom_id'                       '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_tor                            1  '_chem_comp_tor.atom_id_1'                            '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_tor                            2  '_chem_comp_tor.atom_id_2'                            '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_tor                            3  '_chem_comp_tor.atom_id_3'                            '_chem_comp_atom.atom_id'                   chem_comp_atom
chem_comp_tor                            4  '_chem_comp_tor.atom_id_4'                            '_chem_comp_atom.atom_id'                   chem_comp_atom
#endif

    else if (itemName == "_chem_comp_atom.atom_id")
        return (true);

    return (false);
}


bool CifExcept::IsBadChildRelation(const string& itemName)
{
#ifdef BAD_RELATION
refine_ls_restr_ncs                      4  '_refine_ls_restr_ncs.pdbx_auth_asym_id'              '_atom_site.auth_asym_id'                   atom_site
#endif

    if (itemName == "_refine_ls_restr_ncs.pdbx_auth_asym_id")
        return (true);

#ifdef BAD_RELATION
chem_comp_link                           1  '_chem_comp_link.type_comp_1'                         '_chem_comp.type'                           chem_comp
#endif

    else if (itemName == "_chem_comp_link.type_comp_1")
        return (true);

#ifdef BAD_RELATION
chem_comp_link                           2  '_chem_comp_link.type_comp_2'                         '_chem_comp.type'                           chem_comp
#endif

    else if (itemName == "_chem_comp_link.type_comp_2")
        return (true);

#ifdef BAD_RELATION
pdbx_nonpoly_scheme                      1  '_pdbx_nonpoly_scheme.asym_id'                        '_atom_site.label_asym_id'                  atom_site
#endif

    else if (itemName == "_pdbx_nonpoly_scheme.asym_id")
        return (true);

#ifdef BAD_RELATION
pdbx_unobs_or_zero_occ_atoms             1  '_pdbx_unobs_or_zero_occ_atoms.PDB_model_num'         '_atom_site.pdbx_PDB_model_num'             atom_site
#endif

    else if (itemName == "_pdbx_unobs_or_zero_occ_atoms.PDB_model_num")
        return (true);

#ifdef BAD_RELATION
pdbx_unobs_or_zero_occ_residues          1  '_pdbx_unobs_or_zero_occ_residues.PDB_model_num'      '_atom_site.pdbx_PDB_model_num'             atom_site
#endif

    else if (itemName == "_pdbx_unobs_or_zero_occ_residues.PDB_model_num")
        return (true);

#ifdef BAD_RELATION
pdbx_nonpoly_scheme                      1  '_pdbx_nonpoly_scheme.mon_id'                         '_atom_site.label_comp_id'                  atom_site
#endif

    else if (itemName == "_pdbx_nonpoly_scheme.mon_id")
        return (true);

#ifdef BAD_RELATION
pdbx_nonpoly_scheme                      1  '_pdbx_nonpoly_scheme.entity_id'                      '_atom_site.label_entity_id'                atom_site
#endif

    else if (itemName == "_pdbx_nonpoly_scheme.entity_id")
        return (true);

#ifdef BAD_RELATION
pdbx_struct_ref_seq_deletion             1  '_pdbx_struct_ref_seq_deletion.asym_id'               '_pdbx_poly_seq_scheme.asym_id'             pdbx_poly_seq_scheme
#endif

    else if (itemName == "_pdbx_struct_ref_seq_deletion.asym_id")
        return (true);

#ifdef BAD_RELATION
pdbx_struct_sheet_hbond                  4  '_pdbx_struct_sheet_hbond.range_id_1'                 '_struct_sheet_range.id'                    struct_sheet_range
#endif

    else if (itemName == "_pdbx_struct_sheet_hbond.range_id_1")
        return (true);

#ifdef BAD_RELATION
pdbx_struct_sheet_hbond                  5  '_pdbx_struct_sheet_hbond.range_id_2'                 '_struct_sheet_range.id'                    struct_sheet_range
#endif

    else if (itemName == "_pdbx_struct_sheet_hbond.range_id_2")
        return (true);

#ifdef BAD_RELATION
atom_site                                5  '_atom_site.label_atom_id'                            '_chem_comp_atom.atom_id'                   chem_comp_atom
#endif

    else if (itemName == "_atom_site.label_atom_id")
        return (true);

    return (false);
}
