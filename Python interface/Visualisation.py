import dash
import dash_core_components as dcc
import dash_cytoscape as cyto
import dash_html_components as html
from dash.dependencies import Input, Output
from prp import *
from PRP1 import *

app = dash.Dash(__name__)

test = "../PRP_instances/1PDI_Instance"

pathInst ="../PRP_instances/A_04"

prp = PRP(pathInst)

pathInput = dcc.Input(
            id="Path",
            type="text",
            placeholder="path to instance file",
            value=pathInst
        )

maxTimeStep = prp.l

timeStep = dcc.Slider(
     id='timeStep',
    min=1,
    max=maxTimeStep,
    step=1,
    value=1,
    )

graph = html.Div(id="graph",children=prp.toDash())

fileButton = html.Button('fileButton', id='fileButton',n_clicks=0)
prp1Button = html.Button('prp1Button', id='prp1Button',n_clicks=0)

app.layout = html.Div([graph,pathInput,fileButton,prp1Button,timeStep])

@app.callback(
    dash.dependencies.Output('graph','children'),
    [dash.dependencies.Input('fileButton','n_clicks')],
    [dash.dependencies.Input('prp1Button','n_clicks')],
    [dash.dependencies.Input('timeStep', 'value')],
    [dash.dependencies.State('Path', 'value')])
def update_output(nc1,nc2,t,path):
    global pathInst
    changed_id = [p['prop_id'] for p in dash.callback_context.triggered][0]
    pathInst=path
    global prp
    if 'fileButton' in changed_id:
        prp=PRP(path)
        maxTimeStep = prp.l
    elif 'prp1Button' in changed_id:
        prp=PRP1Sol(path)
        maxTimeStep = prp.prp.l
    if path != None:
        return prp.toDash(t)

if __name__ == '__main__':
    app.run_server(debug=True)
