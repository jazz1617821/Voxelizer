#include "RawFile.h"

void saveRaw(const char * filepath, VoxelData * vdata)
{
	const char* rawFilePath = filepath;
	//printf("%s\n", filepath);
	char *infFilePath = (char*)malloc(sizeof(char) * (strlen(filepath)));
	strcpy(infFilePath, filepath);
	char *i = strrchr(infFilePath, '.');
	infFilePath[i - infFilePath + 1] = '\0';
	strcat(infFilePath, "inf");
	//printf("%s\n", str);

	writeInfoFile(infFilePath, vdata);
	writeRawFile(rawFilePath, vdata);

	return;
}

void writeInfoFile(const char * filepath, VoxelData * vdata)
{
	char *infFilePath = (char*)malloc(sizeof(char) * (strlen(filepath)));
	strcpy(infFilePath, filepath);
	char *i = strrchr(infFilePath, '/');
	char *end = strrchr(infFilePath, '\0');
	char *filename = (char*)malloc(sizeof(char) * (end - i));
	const char* a= infFilePath + (i - infFilePath + 1) ;
	strcpy(filename, a);
	//printf("%s", filename);
	char *ab =strrchr(filename, '.');
	filename[ab - filename + 1] = '\0';
	strcat(filename, "raw");
	//printf("%s", filename);



	FILE* fp = fopen(filepath, "w");

	//raw - file = Model.raw
	//resolution = 101x101x101
	//sample - type = float
	//voxel - size = 0.010000:0.010000 : 0.010000
	//ratio:0.10000x0.10000x0.10000
	//original-location=100,200,100
	//endian = little

	fprintf(fp, "raw-file=%s\n", filename);
	fprintf(fp, "resolution=%dx%dx%d\n", vdata->resolution[0], vdata->resolution[1], vdata->resolution[2]);
	fprintf(fp, "sample-type=unsigned char\n");
	fprintf(fp, "voxel-size=%f:%f:%f\n", vdata->voxelSize[0], vdata->voxelSize[1], vdata->voxelSize[2]);
	fprintf(fp, "ratio=%f:%f:%f\n", vdata->voxelSize[0], vdata->voxelSize[1], vdata->voxelSize[2]);
	fprintf(fp, "original-location=%f,%f,%f\n", vdata->model->bbox->min[0], vdata->model->bbox->min[1], vdata->model->bbox->min[2]);
	fprintf(fp, "endian=little\n");

	fclose(fp);
}

void writeRawFile(const char * filepath, VoxelData * vdata)
{
	FILE* fp = fopen(filepath,"wb");

	fwrite(vdata->rawData,sizeof(Voxel),vdata->resolution[0] * vdata->resolution[1] * vdata->resolution[2], fp);

	fclose(fp);
	
	return;
}
