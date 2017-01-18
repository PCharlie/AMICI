%
% @file amised
% @brief definition of amised class
%
classdef amised < handle
    % AMISED is a container for SED-ML objects
    
    properties ( GetAccess = 'public', SetAccess = 'private' )
        % amimodel from the specified model
        model = struct('event',[],'sym',[]);
        % cell array of model identifiers
        modelname = {};
        % stores the struct tree from the xml definition
        sedml = struct.empty();
        % count the number of outputs per model
        outputcount = [];
        % indexes for dataGenerators
        varidx = [];
        varsym = sym([]);
        datasym = sym([]);
        
        javaobj = org.sedml.libsedml.SedDocument;
        
    end
    
    properties ( GetAccess = 'public', SetAccess = 'public' )
 
    end
    
    methods
        function ASED = amised(sedname)

            ASED = ASED.importSEDML(sedname);
            
            ASED.modelname = char(ASED.javaobj.getName);
            
            % get models
            for imodel = 1:length(ASED.javaobj.getListOfModels.getNumModels)
                % get the model sbml
                % check if this is a biomodels model
                
                source = char(ASED.javaobj.getModel(imodel-1).getSource);
                if(length(source)>=23)
                    if(strcmp(source,ASED.modelname))
                        ASED.model(imodel) = ASED.model(find(strcmp(source,ASED.modelname)));
                        ASED.modelname{imodel} = ASED.sedml.listOfModels.model{imodel}.Attributes.id;
                        ASED.model(imodel).sym.y = sym([]);
                        ASED.outputcount(imodel) = 0;
                        continue
                    end
                    if(strcmp(ASED.sedml.listOfModels.model{imodel}.Attributes.source(1:23),'urn:miriam:biomodels.db'))
                        modelxml = websave([ASED.sedml.listOfModels.model{imodel}.Attributes.source(25:end) '.xml'],['http://www.ebi.ac.uk/biomodels-main/download?mid=' ASED.sedml.listOfModels.model{imodel}.Attributes.source(25:end)]);
                    else
                        modelxml = ASED.sedml.listOfModels.model{imodel}.Attributes.source;
                    end
                else
                    modelxml = source;
                end
                modelxml = fullfile(fileparts(sedname),strrep(modelxml,'.xml',''));
                % convert model sbml to amici
                
                model_id = char(ASED.javaobj.getModel(0).getId);
                SBML2AMICI(modelxml, [ASED.modelname '_' model_id]);
                eval(['model = ' ASED.modelname '_' model_id '_syms();'])
                % clear output;
                ASED.model(imodel).sym.y = sym([]);
                ASED.outputcount(imodel) = 0;
                ASED.modelname{imodel} = model_id;
            end
            % apply changes
            if(isfield(ASED.sedml,'listOfChanges'))
                %TBD apply changes
            end
            % construct outputs
            for idata = 1:length(ASED.sedml.listOfDataGenerators.dataGenerator)
                if(length(ASED.sedml.listOfDataGenerators.dataGenerator)>1)
                    dataGenerator = ASED.sedml.listOfDataGenerators.dataGenerator{idata};
                else
                    dataGenerator = ASED.sedml.listOfDataGenerators.dataGenerator;
                end
                tasks = [ASED.sedml.listOfTasks.task{:}];
                tasksatts = [tasks.Attributes];
                taskids = {tasksatts.id};
                for ivar = 1:length(dataGenerator.listOfVariables.variable)
                    if(length(dataGenerator.listOfVariables.variable)>1)
                        variable = dataGenerator.listOfVariables.variable{ivar};
                    else
                        variable = dataGenerator.listOfVariables.variable;
                    end
                    task_id = find(strcmp(variable.Attributes.taskReference,taskids));
                    if(isempty(task_id))
                        error(['Invalid taskReference in dataGenerator ' num2str(idata) ': ' variable.Attributes.taskReference]);
                    end
                    model_idx = find(strcmp(ASED.sedml.listOfTasks.task{task_id}.Attributes.modelReference,ASED.modelname));
                    if(isempty(model_idx))
                        error(['Invalid modelReference in task ' num2str(task_id) ': ' ASED.sedml.listOfTasks.task{task_id}.Attributes.modelReference]);
                    end
                    if(isfield(variable.Attributes,'symbol'))
                        if(strcmp(variable.Attributes.symbol,'urn:sedml:symbol:time'))
                            ASED.model(model_idx).sym.y(ASED.outputcount(model_idx)+1) = sym('t');
                        end
                    end
                    if(isfield(variable.Attributes,'target'))
                        if(strfind(variable.Attributes.target,'/sbml:sbml/sbml:model/sbml:listOfSpecies/sbml:species'))
                            ASED.model(model_idx).sym.y(ASED.outputcount(model_idx)+1) = sym(variable.Attributes.target(60:end-2));
                        end
                    end
                    ASED.outputcount(model_idx) = ASED.outputcount(model_idx)+1;
                    ASED.varidx(idata,ivar,:) = [model_idx,ASED.outputcount(model_idx)];
                    ASED.varsym(idata,ivar) = sym(variable.Attributes.id);
                end
                ASED.datasym(idata) = sym(variable.Attributes.id);
                
            end

            
        end
        
        function ASED = importSEDML(this,sedname)
            
            loadsedml;
            import org.sedml.libsedml.*
            ASED.javaobj = libsedml.readSedML(sedname);
            
        end
        
    end
end

