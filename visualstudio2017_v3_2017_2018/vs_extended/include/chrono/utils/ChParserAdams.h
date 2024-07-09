// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2017 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Conlain Kelly
// =============================================================================
//
// Parser utility class for ADMAS .adm input files.
//
// =============================================================================

#ifndef CH_PARSER_ADAMS_H
#define CH_PARSER_ADAMS_H

#include <functional>
#include <map>
#include <sstream>

#include "chrono/core/ChApiCE.h"
#include "chrono/physics/ChBodyAuxRef.h"
#include "chrono/physics/ChSystem.h"

enum TokenType {
    MARKER,
    LABEL,
    VALUE,
    COMMA,
    GRAPHICS,
    PAREN_O,
    PAREN_C,
    NEWLINE,
    EQUALS,
    COMMENT,
    NEGATE,
    ADAMS,
    UNITS,
    PART,
    JOINT,
    REQUEST,
    COLON,
    BACKSLASH,
    ACCGRAV,
    OUTPUT,
    END,
    DELIMITER  // used for primary tokens -- things that are directly represented in Chrono
};

namespace chrono {

namespace utils {

/// @addtogroup chrono_utils
/// @{

/// ADAMS input file parser
class ChApi ChParserAdams {
  public:
    // Use the vis assets loaded from ADAMS or not
    // could also be modified to add a different visualization technique
    enum VisType { LOADED, NONE };

    ChParserAdams() : m_visType(VisType::LOADED), m_verbose(false) {}
    ~ChParserAdams() {}

    /// Report containing information about objects parsed from file
    class ChApi Report {
      public:
        /// Information about a joint read in from ADAMS.
        struct JointInfo {
            std::string type;               ///< joint type as shown in adm file
            std::shared_ptr<ChLink> joint;  ///< Chrono link (joint)
        };

        std::unordered_map<std::string, std::shared_ptr<ChBodyAuxRef>> bodies;  ///< list of body information
        std::unordered_map<std::string, JointInfo> joints;                      ///< list of joint information

        /// Print information on all modeling elements parsed from adm file.
        void Print() const;

        /// Get a handle to the body with specified name.
        /// If none exists, an empty shared pointer is returned.
        /// Note that all bodies created by the parser are of type ChBodyAuxRef
        /// (i.e., using a non-centroidal reference frame).
        std::shared_ptr<ChBodyAuxRef> GetBody(const std::string& name) const;

        /// Get a handle to the joint with specified name.
        /// If none exists, an empty shared pointer is returned.
        /// The caller may need to downcast to the appropriate type.
        std::shared_ptr<ChLink> GetJoint(const std::string& name) const;
    };

    /// Set body visualization type (default: NONE).
    void SetVisualizationType(VisType val) { m_visType = val; }

    /// Enable/disable verbose parsing output (default: false).
    void SetVerbose(bool val) { m_verbose = val; }

    /// Parse the specified Adams input file and create the model in the given system.
    void Parse(ChSystem& sys,               ///< [in] containing Chrono system
               const std::string& filename  ///< [in] Adams input file name
    );

    /// Parse the specified Adams input file and create the model in a new system.
    /// Note that the created system is not deleted in the parser's destructor;
    /// rather, ownership is transferred to the caller.
    ChSystem* Parse(const std::string& filename,  ///< [in] Adams input file name
                    ChMaterialSurface::ContactMethod contact_method = ChMaterialSurface::NSC  ///< [in] contact method
    );

    /// Get the list of bodies in the model.
    const std::vector<std::shared_ptr<ChBodyAuxRef>>& GetBodyList() const { return m_bodyList; }

    /// Get the list of joints in the model.
    const std::vector<std::shared_ptr<ChLink>>& GetJointList() const { return m_jointList; }

    // Generated by the FLEX stuff, this is an external function that tokenizes things
    int yylex();
    // Holds a list of tokens created by yylex()
    // This should be public so flex can get to it maybe?
    std::vector<std::pair<int, std::string>> m_tokens;

    /// Get the report for this parser.
    /// This contains the lists of bodies, joints, and forces that were created from the input osim file.
    const Report& GetReport() const { return m_report; }

    /// Print the parser's report.
    void PrintReport() const { m_report.Print(); }

  private:
    Report m_report;

    // Get an STL vector of tokens from a file, used to make the xml parsing cleaner
    void tokenize(const std::string& filename);

    template <typename T>
    static inline std::vector<T> strToSTLVector(const char* string) {
        std::istringstream buf(string);
        std::istream_iterator<T> beg(buf), end;
        return std::vector<T>(beg, end);
    }

    bool m_verbose;     ///< verbose output
    VisType m_visType;  ///< Body visualization type

    // No contact in ADAMS parser so far
    // bool m_collide;     ///< Do bodies have collision shapes?
    // int m_family_1;     ///< First collision family
    // int m_family_2;     ///< Second collision family

    // float m_friction;       ///< contact coefficient of friction
    // float m_restitution;    ///< contact coefficient of restitution
    // float m_young_modulus;  ///< contact material Young modulus
    // float m_poisson_ratio;  ///< contact material Poisson ratio
    // float m_kn;             ///< normal contact stiffness
    // float m_gn;             ///< normal contact damping
    // float m_kt;             ///< tangential contact stiffness
    // float m_gt;             ///< tangential contact damping

    std::vector<std::shared_ptr<ChBodyAuxRef>> m_bodyList;  ///< List of bodies in model
    std::vector<std::shared_ptr<ChLink>> m_jointList;       ///< List of joints in model
};

// Capture yylex for our use
#define YY_DECL int chrono::utils::ChParserAdams::yylex()

/// @} chrono_utils

}  // end namespace utils
}  // end namespace chrono

#endif
