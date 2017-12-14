# docker-cyverse-irods-rs

An iRODS resource server that runs in a Docker container. The container is
configured for the CyVerse Data Store.


## Generating the Docker Image

The `build-cyverse-rs` program can be used to generate a Docker image containing
an iRODS resource server that is configured to serve a given resource within the
CyVerse Data Store. It also generates the `irods-svc` script. This script can be
used to start and stop a container with the resource running in it. `irods-svc
start <env-file>` creates and starts a detached container named `cyverse-rs`
with the resource server running where `<env-file>` is the name of a file
holding required environment variables. `irods-svc stop` stops and removes the
`cyverse-rs` container.

As its first command line argument, `build-cyverse-rs` expects the tag that will
be assigned to the generated imaged. It also accepts an optional second argument
specifying the directory where the  `irods-svc` script is to be written. If this
isn't provided, the scripts will be written to the current working directory.

The `build-cyverse-rs` expects several environment variables to be defined in an
environment file when it is executed.

Environment Variable      | Required | Default       | Description
------------------------- | -------- | ------------- | -----------
`CYVERSE_DS_CLERVER_USER` | no       | ipc_admin     | the name of the rodsadmin user representing the resource server within the zone
`CYVERSE_DS_LOG_DIR`      | no       | `$HOME`/log   | the host directory where the container will mount the iRODS log directory (`/var/lib/irods/iRODS/server/log`), `$HOME` is evaluated at container start time
`CYVERSE_DS_RES_NAME`     | yes      |               | the name of the storage resource that will be served
`CYVERSE_DS_RES_SERVER`   | yes      |               | the FQDN or address used by the rest of the grid to communicate with this server
`CYVERSE_DS_RES_VAULT`    | no       | `$HOME`/vault | the host directory where the container will mount the vault, for the default, `$HOME` is evaluated at container start time

Here's an example.

```
prompt> cat build.env
CYVERSE_DS_RES_NAME=rs
CYVERSE_DS_RES_SERVER=rs.domain.net

prompt> build-cyverse-rs rs-tag build.env scriptDir

prompt> ls scriptDir
irods-svc
```

When starting iRODS, the `irods-svc` script requires the following environment
variables defined.

Environment Variable           | Description
------------------------------ | -----------
`CYVERSE_DS_CLERVER_PASSWORD`  | the password used to authenticate `CYVERSE_DS_CLERVER_USER`
`CYVERSE_DS_CONTROL_PLANE_KEY` | the encryption key required for communicating over the relevant iRODS grid control plane
`CYVERSE_DS_NEGOTIATION_KEY`   | the encryption key shared by the iplant zone for advanced negotiation during client connections
`CYVERSE_DS_ZONE_KEY`          | the shared secret used during server-to-server communication

Here's an example.

```
prompt> cat run.env
###
# *** DO NOT SHARE THIS FILE ***
#
# THIS FILE CONTAINS SECRET INFORMATION THAT COULD BE USED TO GAIN PRIVILEGED
# ACCESS TO THE CYVERSE DATA STORE. PLEASE KEEP THIS FILE IN A SECURE PLACE.
#
###
CYVERSE_DS_CLERVER_PASSWORD=SECRET_PASSWORD
CYVERSE_DS_CONTROL_PLANE_KEY=SECRET_____32_byte_ctrl_plane_key
CYVERSE_DS_NEGOTIATION_KEY=SECRET____32_byte_negotiation_key
CYVERSE_DS_ZONE_KEY=SECRET_zone_key

prompt> scriptDir/irods-svc start run.env
```

## Prerequisites

Before this a generated image can be used, there are a few things that need to
be done first.

First, the storage resource needs to be defined within the CyVerse Data Store.
The vault path within the container will be a subdirectory of `/irods_vault`
with the same name as the storage resource being served, i.e.,
`/irods_vault/"$CYVERSE_DS_RES_NAME"`. This can be done with a command like the
following.

```
iadmin mkresc "$CYVERSE_DS_RES_NAME" 'unix file system' \
              "$CYVERSE_DS_RES_SERVER":/irods_vault/"$CYVERSE_DS_RES_NAME"
```

Next, the corresponding coordinating resource needs to be created for the
storage resource. The name of the coordinate resource needs to be the name of
the storage resource with `Res` appended, i.e., `"$CYVERSE_DS_RES_NAME"Res`.
This will be the default resource served by the resource server. This can be
done with a set of commands like the following.

```
coordRes="$CYVERSE_DS_RES_NAME"Res
iadmin mkresc "$coordRes" passthru
iadmin addchildtoresc "$coordRes" "$CYVERSE_DS_RES_NAME"
```

Finally, the vault directory on the server hosting the container needs to
exist and be writable by the user that will be running the container.


## Repository Dependencies

This repository has two subtrees. The master branch of the
https://github.com/cyverse/irods-netcdf-build is attached to the directory
`base/irods-netcdf-build`. The master branch of the
https://github.com/iPlantCollaborativeOpenSource/irods-setavu-plugin is attached
to the directory `base/irods-setavu-plugin`.
