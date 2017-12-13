// =-=-=-=-=-=-=-
#include "msParam.hpp"
#include "reGlobalsExtern.hpp"
#include "irods_ms_plugin.hpp"
#include "netcdfMS.hpp"

// =-=-=-=-=-=-=-
// STL Includes
#include <iostream>

extern "C" {

    // =-=-=-=-=-=-=-
    // 1. Set the version of the plugin.  1.0 is currently the standard

    // =-=-=-=-=-=-=-
    // 2. Write a standard issue microservice

    /**
    * \fn msiFreeNcStruct (msParam_t *inpParam, ruleExecInfo_t *rei)
    ** \brief Free the NcGetVarOut_MS_T and its content
    * \module core
    *
    * \since 3.2
    *
    * \author  Mike Wan
    * \date    2012
    *
    * \usage See clients/icommands/test/rules3.0/netcdfTest1.r, netcdfTest2.r and netcdfTest3.r.
    * \param[in] inpParam - A NcGetVarOut_MS_T to free
    * \param[out] None
    *
    * \DolVarDependence none
    * \DolVarModified none
    * \iCatAttrDependence none
    * \iCatAttrModified none
    * \sideeffect none
    *
    * \return integer
    * \retval 0 upon success
    * \pre N/A
    * \post N/A
    * \sa N/A
    *
    **/
    int
    msiFreeNcStruct (msParam_t *inpParam, ruleExecInfo_t *rei)
    {
        ncGetVarOut_t *ncArray;

        RE_TEST_MACRO ("    Calling msiFreeNcStruct")

        if (inpParam == NULL) return USER__NULL_INPUT_ERR;

        if (strcmp (inpParam->type, NcGetVarOut_MS_T) == 0) {
            ncArray = (ncGetVarOut_t *) inpParam->inOutStruct;
            if (ncArray != NULL) {
                freeNcGetVarOut (&ncArray);
                inpParam->inOutStruct = NULL;
            }
        } else {
            rodsLog (LOG_ERROR,
              "msiFreeNcStruct: inpParam must be NcGetVarOut_MS_T. %s",
              inpParam->type);
            return (USER_PARAM_TYPE_ERR);
        }

        return 0;
    }

    // =-=-=-=-=-=-=-
    // 3.  Create the plugin factory function which will return a microservice
    //     table entry
    irods::ms_table_entry*  plugin_factory( ) {
        // =-=-=-=-=-=-=-
        // 4.  allocate a microservice plugin which takes the number of function
        //     params as a parameter to the constructor
        irods::ms_table_entry* msvc = new irods::ms_table_entry( 1 );

        // =-=-=-=-=-=-=-
        // 5. add the microservice function as an operation to the plugin
        //    the first param is the name / key of the operation, the second
        //    is the name of the function which will be the microservice
        msvc->add_operation( "msiFreeNcStruct", "msiFreeNcStruct" );

        // =-=-=-=-=-=-=-
        // 6. return the newly created microservice plugin
        return msvc;
    }

}; // extern "C"



